/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2019 Tobias Heuer <tobias.heuer@kit.edu>
 * Copyright (C) 2019 Lars Gottesbüren <lars.gottesbueren@kit.edu>
 *
 * KaHyPar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KaHyPar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KaHyPar.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#pragma once

#include <atomic>
#include <type_traits>

#include "tbb/parallel_for.h"

#include "mt-kahypar/macros.h"
#include "mt-kahypar/definitions.h"
#include "mt-kahypar/parallel/atomic_wrapper.h"
#include "mt-kahypar/parallel/stl/scalable_vector.h"

namespace mt_kahypar {
namespace ds {

template<class HyperGraph>
class ConcurrentUnionFind {

 public:
  explicit ConcurrentUnionFind(const HyperGraph& hypergraph) :
    _num_distinct_sets(hypergraph.initialNumNodes()),
    _set(hypergraph.initialNumNodes()),
    _contains_high_degree_vertex(hypergraph.initialNumNodes(), 0) {
    tbb::parallel_for(0UL, hypergraph.initialNumNodes(), [&](const HypernodeID id) {
      const HypernodeID hn = hypergraph.globalNodeID(id);
      if ( hypergraph.nodeIsEnabled(hn) ) {
        _set[id] = -hypergraph.nodeWeight(hn);
        _contains_high_degree_vertex[id] = hypergraph.isHighDegreeVertex(hn);
      } else {
        _set[id] = std::numeric_limits<HypernodeWeight>::min();
        --_num_distinct_sets;
      }
    });
  }

  size_t numDistinctSets() const {
    return _num_distinct_sets.load();
  }

  void link(const HypernodeID u, const HypernodeID v) {
    ASSERT(u < _set.size());
    ASSERT(v < _set.size());

    bool success = false;
    while ( !success ) {
      HypernodeWeight root_u = static_cast<HypernodeWeight>(find(u));
      HypernodeWeight weight_u = _set[root_u].load();
      HypernodeWeight root_v = static_cast<HypernodeWeight>(find(v));
      HypernodeWeight weight_v = _set[root_v].load();

      if (weight_u < 0 && weight_v < 0) {
        if ( root_u == root_v ) {
          success = true;
        } else if ( weight_u > weight_v ) {
          if ( _set[root_u].compare_and_exchange_strong(weight_u, root_v) ) {
            HypernodeWeight desired_weight = weight_u + weight_v;
            while ( !_set[root_v].compare_and_exchange_strong(weight_v, desired_weight) ) {
              root_v = static_cast<HypernodeWeight>(find(v));
              weight_v = _set[root_v].load();
              desired_weight = weight_u + weight_v;
            }
            _contains_high_degree_vertex[root_v] |= _contains_high_degree_vertex[root_u];
            --_num_distinct_sets;
            success = true;
          }
        } else /* |weight_u| >= |weight_v| */ {
          if ( _set[root_v].compare_and_exchange_strong(weight_v, root_u) ) {
            HypernodeWeight desired_weight = weight_u + weight_v;
            while ( !_set[root_u].compare_and_exchange_strong(weight_u, desired_weight) ) {
              root_u = static_cast<HypernodeWeight>(find(u));
              weight_u = _set[root_u].load();
              desired_weight = weight_u + weight_v;
            }
            _contains_high_degree_vertex[root_u] |= _contains_high_degree_vertex[root_v];
            --_num_distinct_sets;
            success = true;
          }
        }
      }
    }
  }

  bool isSameSet(const HypernodeID u, const HypernodeID v) {
    ASSERT(u < _set.size());
    ASSERT(v < _set.size());
    return find(u) == find(v);
  }

  HypernodeID find(const HypernodeID u) {
    ASSERT(u < _set.size());
    HypernodeWeight parent = _set[u];
    if ( parent < 0 /* u is root */ ) {
      return u;
    }

    ASSERT(parent < static_cast<HypernodeWeight>(_set.size()));
    HypernodeWeight root = static_cast<HypernodeWeight>(find(parent));
    if ( parent != root ) {
      _set[u].compare_and_exchange_strong(parent, root);
    }
    return root;
  }

  HypernodeWeight weight(const HypernodeID u) {
    ASSERT(u < _set.size());
    HypernodeWeight weight = 0;
    while ( weight >= 0 ) {
      // It might happen that between find(u) and
      // _set[root].load(), u becomes an child of an
      // other vertex and therefore it contains no valid
      // weight information.
      const HypernodeID root = find(u);
      ASSERT(root < _set.size());
      weight = _set[root].load();
    }
    return std::abs(weight);
  }

  bool containsHighDegreeVertex(const HypernodeID u) {
    ASSERT(u < _set.size());
    return _contains_high_degree_vertex[find(u)];
  }

 private:
  std::atomic<size_t> _num_distinct_sets;
  parallel::scalable_vector<parallel::IntegralAtomicWrapper<HypernodeWeight>> _set;
  parallel::scalable_vector<uint8_t> _contains_high_degree_vertex;
};

}  // namespace ds
}  // namespace mt_kahypar
