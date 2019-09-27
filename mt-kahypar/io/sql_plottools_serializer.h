/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2014-2016 Sebastian Schlag <sebastian.schlag@kit.edu>
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
#include <array>
#include <chrono>
#include <fstream>
#include <string>

#include "mt-kahypar/definitions.h"
#include "mt-kahypar/partition/context.h"
#include "mt-kahypar/utils/timer.h"

namespace mt_kahypar {
namespace io {
namespace serializer {
static inline void serialize(const Hypergraph& hypergraph, const Context& context) {
  if (context.partition.sp_process_output) {
    std::ostringstream oss;
    oss << "RESULT"
        << " graph=" << context.partition.graph_filename.substr(
        context.partition.graph_filename.find_last_of('/') + 1)
        << " numHNs=" << hypergraph.initialNumNodes()
        << " numHEs=" << hypergraph.initialNumEdges()
        << " mode=" << context.partition.mode
        << " objective=" << context.partition.objective
        << " k=" << context.partition.k
        << " epsilon=" << context.partition.epsilon
        << " seed=" << context.partition.seed
        << " he_size_threshold=" << context.partition.hyperedge_size_threshold
        << " total_graph_weight=" << hypergraph.totalWeight()
        << " coarsening_algorithm=" << context.coarsening.algorithm
        << " contraction_limit_multiplier=" << context.coarsening.contraction_limit_multiplier
        << " max_allowed_weight_multiplier=" << context.coarsening.max_allowed_weight_multiplier
        << " max_allowed_node_weight=" << context.coarsening.max_allowed_node_weight
        << " contraction_limit=" << context.coarsening.contraction_limit
        << " hypernode_weight_fraction=" << context.coarsening.hypernode_weight_fraction
        << " rating_function=" << context.coarsening.rating.rating_function
        << " rating_heavy_node_penalty_policy=" << context.coarsening.rating.heavy_node_penalty_policy
        << " rating_acceptance_policy=" << context.coarsening.rating.acceptance_policy
        << " num_threads=" << context.shared_memory.num_threads
        << " use_community_redistribution=" << std::boolalpha << context.shared_memory.use_community_redistribution
        << " community_assignment_strategy=" << context.shared_memory.assignment_strategy
        << " community_assignment_objective=" << context.shared_memory.assignment_objective;

    utils::Timer::instance(context.partition.detailed_timings).serialize(oss);

    std::cout << oss.str() << std::endl;
  }
}
}  // namespace serializer
}  // namespace io
}  // namespace mt_kahypar