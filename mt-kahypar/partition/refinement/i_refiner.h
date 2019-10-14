/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2014 Sebastian Schlag <sebastian.schlag@kit.edu>
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
#include <string>
#include <utility>
#include <vector>

#include "kahypar/macros.h"
#include "kahypar/partition/metrics.h"

#include "mt-kahypar/definitions.h"

namespace mt_kahypar {

class IRefiner {
 public:
  IRefiner(const IRefiner&) = delete;
  IRefiner(IRefiner&&) = delete;
  IRefiner& operator= (const IRefiner&) = delete;
  IRefiner& operator= (IRefiner&&) = delete;

  virtual ~IRefiner() = default;

  bool refine(std::vector<HypernodeID>& refinement_nodes,
              kahypar::Metrics& best_metrics) {
    return refineImpl(refinement_nodes, best_metrics);
  }


 protected:
  IRefiner() = default;

 private:
  virtual bool refineImpl(std::vector<HypernodeID>& refinement_nodes,
                          kahypar::Metrics& best_metrics) = 0;
};

}  // namespace mt_kahypar