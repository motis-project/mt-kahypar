/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2021 Tobias Heuer <tobias.heuer@kit.edu>
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

#include "mt-kahypar/partition/refinement/advanced/flows/flow_refiner.h"

#include "tbb/concurrent_queue.h"

namespace mt_kahypar {

MoveSequence FlowRefiner::refineImpl(const PartitionedHypergraph& phg,
                                     const Subhypergraph& sub_hg,
                                     const HighResClockTimepoint& start) {
  MoveSequence sequence { { }, 0 };
  // Construct flow network that contains all vertices given in refinement nodes
  utils::Timer::instance().start_timer("construct_flow_problem", "Construct Flow Problem", true);
  FlowProblem flow_problem = constructFlowHypergraph(phg, sub_hg);
  utils::Timer::instance().stop_timer("construct_flow_problem");
  if ( flow_problem.total_cut - flow_problem.non_removable_cut > 0 ) {
    // Set maximum allowed block weights for block 0 and 1
    _hfc.cs.setMaxBlockWeight(0, std::max(
      flow_problem.weight_of_block_0, _context.partition.max_part_weights[_block_0]));
    _hfc.cs.setMaxBlockWeight(1, std::max(
      flow_problem.weight_of_block_1, _context.partition.max_part_weights[_block_1]));

    _hfc.reset();
    _hfc.upperFlowBound = flow_problem.total_cut - flow_problem.non_removable_cut;
    // Solve max-flow min-cut problem
    bool time_limit_reached = false;
    utils::Timer::instance().start_timer("solve_flow_problem", "Solve Flow Problem", true);
    bool flowcutter_succeeded = computeFlow(flow_problem, start, time_limit_reached);
    utils::Timer::instance().stop_timer("solve_flow_problem");
    if ( flowcutter_succeeded ) {
      // We apply the solution if it either improves the cut or the balance of
      // the bipartition induced by the two blocks
      HyperedgeWeight new_cut = flow_problem.non_removable_cut + _hfc.cs.flowValue;
      const bool improved_solution = new_cut < flow_problem.total_cut ||
        ( new_cut == flow_problem.total_cut &&
          static_cast<HypernodeWeight>(std::max(_hfc.cs.n.sourceWeight, _hfc.cs.n.targetWeight)) <
          std::max(flow_problem.weight_of_block_0, flow_problem.weight_of_block_1));

      // Extract move sequence
      if ( improved_solution ) {
        sequence.expected_improvement = flow_problem.total_cut - new_cut;
        for ( const whfc::Node& u : _flow_hg.nodeIDs() ) {
          const HypernodeID hn = _whfc_to_node[u];
          if ( hn != kInvalidHypernode ) {
            const PartitionID from = phg.partID(hn);
            const PartitionID to = _hfc.cs.n.isSource(u) ? _block_0 : _block_1;
            if ( from != to ) {
              sequence.moves.push_back(Move { from, to, hn, kInvalidGain });
            }
          }
        }
      }
    } else if ( time_limit_reached ) {
      sequence.state = MoveSequenceState::TIME_LIMIT;
    }
  }
  return sequence;
}

#define NOW std::chrono::high_resolution_clock::now()
#define RUNNING_TIME(X) std::chrono::duration<double>(NOW - X).count();

bool FlowRefiner::computeFlow(const FlowProblem& flow_problem,
                              const HighResClockTimepoint& start,
                              bool& time_limit_reached) {
  whfc::Node s = flow_problem.source;
  whfc::Node t = flow_problem.sink;
  _hfc.cs.initialize(s, t);
  bool piercingFailedOrFlowBoundReachedWithNonAAPPiercingNode = false;
  bool has_balanced_cut = false;

  size_t iteration = 0;
  time_limit_reached = false;
  while (!time_limit_reached && _hfc.cs.flowValue <= _hfc.upperFlowBound && !has_balanced_cut) {
    piercingFailedOrFlowBoundReachedWithNonAAPPiercingNode =
      !_hfc.advanceOneFlowIteration(_hfc.cs.flowValue == _hfc.upperFlowBound);
    if (piercingFailedOrFlowBoundReachedWithNonAAPPiercingNode)
      break;
    has_balanced_cut = _hfc.cs.hasCut && _hfc.cs.isBalanced(); //no cut ==> run and don't check for balance.

    if ( iteration % 25 == 0 ) {
      const double elapsed_time = RUNNING_TIME(start);
      time_limit_reached = elapsed_time > _time_limit;
    }
    ++iteration;
  }

  if (has_balanced_cut && _hfc.cs.flowValue <= _hfc.upperFlowBound) {
    ASSERT(_hfc.cs.sideToGrow() == _hfc.cs.currentViewDirection());
    const double imb_S_U_ISO =
      static_cast<double>(_hfc.hg.totalNodeWeight() -
      _hfc.cs.n.targetReachableWeight) /
      static_cast<double>(_hfc.cs.maxBlockWeight(_hfc.cs.currentViewDirection()));
    const double imb_T =
      static_cast<double>(_hfc.cs.n.targetReachableWeight) /
      static_cast<double>(_hfc.cs.maxBlockWeight(_hfc.cs.oppositeViewDirection()));
    const bool better_balance_impossible =
      _hfc.cs.unclaimedNodeWeight() == 0 || imb_S_U_ISO <= imb_T;
    if (_hfc.find_most_balanced && !better_balance_impossible) {
      _hfc.mostBalancedCut();
    }
    else {
      _hfc.cs.writePartition();
    }

    _hfc.cs.verifyCutInducedByPartitionMatchesFlowValue();
  }

  // Turn back to initial view direction
  if (_hfc.cs.currentViewDirection() != 0) {
    _hfc.cs.flipViewDirection();
  }

  return !piercingFailedOrFlowBoundReachedWithNonAAPPiercingNode &&
    _hfc.cs.flowValue <= _hfc.upperFlowBound && has_balanced_cut;
}

FlowProblem FlowRefiner::constructFlowHypergraph(const PartitionedHypergraph& phg,
                                                 const Subhypergraph& sub_hg) {
  _block_0 = sub_hg.block_0;
  _block_1 = sub_hg.block_1;
  ASSERT(_block_0 != kInvalidPartition && _block_1 != kInvalidPartition);
  FlowProblem flow_problem;

  if ( _context.refinement.advanced.num_threads_per_search > 1 ) {
    flow_problem = _parallel_construction.constructFlowHypergraph(
      phg, sub_hg, _block_0, _block_1, _whfc_to_node);
  } else {
    flow_problem = _sequential_construction.constructFlowHypergraph(
      phg, sub_hg, _block_0, _block_1, _whfc_to_node);
  }

  DBG << "Flow Hypergraph [ Nodes =" << _flow_hg.numNodes()
      << ", Edges =" << _flow_hg.numHyperedges()
      << ", Pins =" << _flow_hg.numPins()
      << ", Blocks = (" << _block_0 << "," << _block_1 << ") ]";

  return flow_problem;
}
} // namespace mt_kahypar