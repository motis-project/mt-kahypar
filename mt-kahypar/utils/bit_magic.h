/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2019 Tobias Heuer <tobias.heuer@kit.edu>
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

namespace mt_kahypar {
namespace utils {

static constexpr uint8_t bitmask[9] = {
  0,   // 00000000
  1,   // 00000001
  2,   // 00000010
  4,   // 00000100
  8,   // 00001000
  16,  // 00010000
  32,  // 00100000
  64,  // 01000000
  128  // 10000000
};

// Lookup-Table for the number of set bits in a 8-bit word
static constexpr int32_t count[256] =
 { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3,
   3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4,
   3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3,
   3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5,
   3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3,
   3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4,
   3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3,
   3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6,
   6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4,
   3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
   4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5,
   3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5,
   5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4,
   3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4,
   4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
   3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6,
   6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7,
   6, 7, 7, 8 };

// Lookup-Table of the i-th one in a 8-bit word
// Note, in case the i-th one does not exists than
// the result will be 8
static constexpr int32_t select[256][9] =
  { { 8, 8, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 1, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 8, 8, 8, 8, 8, 8 },
    { 8, 2, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 8, 8, 8, 8, 8, 8 },
    { 8, 1, 2, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 8, 8, 8, 8, 8 },
    { 8, 3, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 8, 8, 8, 8, 8, 8 },
    { 8, 1, 3, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 8, 8, 8, 8, 8 },
    { 8, 2, 3, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 8, 8, 8, 8, 8 },
    { 8, 1, 2, 3, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 8, 8, 8, 8 },
    { 8, 4, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 4, 8, 8, 8, 8, 8, 8 },
    { 8, 1, 4, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 4, 8, 8, 8, 8, 8 },
    { 8, 2, 4, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 4, 8, 8, 8, 8, 8 },
    { 8, 1, 2, 4, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 4, 8, 8, 8, 8 },
    { 8, 3, 4, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 4, 8, 8, 8, 8, 8 },
    { 8, 1, 3, 4, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 4, 8, 8, 8, 8 },
    { 8, 2, 3, 4, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 4, 8, 8, 8, 8 },
    { 8, 1, 2, 3, 4, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 4, 8, 8, 8 },
    { 8, 5, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 5, 8, 8, 8, 8, 8, 8 },
    { 8, 1, 5, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 5, 8, 8, 8, 8, 8 },
    { 8, 2, 5, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 5, 8, 8, 8, 8, 8 },
    { 8, 1, 2, 5, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 5, 8, 8, 8, 8 },
    { 8, 3, 5, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 5, 8, 8, 8, 8, 8 },
    { 8, 1, 3, 5, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 5, 8, 8, 8, 8 },
    { 8, 2, 3, 5, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 5, 8, 8, 8, 8 },
    { 8, 1, 2, 3, 5, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 5, 8, 8, 8 },
    { 8, 4, 5, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 4, 5, 8, 8, 8, 8, 8 },
    { 8, 1, 4, 5, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 4, 5, 8, 8, 8, 8 },
    { 8, 2, 4, 5, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 4, 5, 8, 8, 8, 8 },
    { 8, 1, 2, 4, 5, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 4, 5, 8, 8, 8 },
    { 8, 3, 4, 5, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 4, 5, 8, 8, 8, 8 },
    { 8, 1, 3, 4, 5, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 4, 5, 8, 8, 8 },
    { 8, 2, 3, 4, 5, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 4, 5, 8, 8, 8 },
    { 8, 1, 2, 3, 4, 5, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 4, 5, 8, 8 },
    { 8, 6, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 1, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 6, 8, 8, 8, 8, 8 },
    { 8, 2, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 6, 8, 8, 8, 8, 8 },
    { 8, 1, 2, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 6, 8, 8, 8, 8 },
    { 8, 3, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 6, 8, 8, 8, 8, 8 },
    { 8, 1, 3, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 6, 8, 8, 8, 8 },
    { 8, 2, 3, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 6, 8, 8, 8, 8 },
    { 8, 1, 2, 3, 6, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 6, 8, 8, 8 },
    { 8, 4, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 4, 6, 8, 8, 8, 8, 8 },
    { 8, 1, 4, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 4, 6, 8, 8, 8, 8 },
    { 8, 2, 4, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 4, 6, 8, 8, 8, 8 },
    { 8, 1, 2, 4, 6, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 4, 6, 8, 8, 8 },
    { 8, 3, 4, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 4, 6, 8, 8, 8, 8 },
    { 8, 1, 3, 4, 6, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 4, 6, 8, 8, 8 },
    { 8, 2, 3, 4, 6, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 4, 6, 8, 8, 8 },
    { 8, 1, 2, 3, 4, 6, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 4, 6, 8, 8 },
    { 8, 5, 6, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 5, 6, 8, 8, 8, 8, 8 },
    { 8, 1, 5, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 5, 6, 8, 8, 8, 8 },
    { 8, 2, 5, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 5, 6, 8, 8, 8, 8 },
    { 8, 1, 2, 5, 6, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 5, 6, 8, 8, 8 },
    { 8, 3, 5, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 5, 6, 8, 8, 8, 8 },
    { 8, 1, 3, 5, 6, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 5, 6, 8, 8, 8 },
    { 8, 2, 3, 5, 6, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 5, 6, 8, 8, 8 },
    { 8, 1, 2, 3, 5, 6, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 5, 6, 8, 8 },
    { 8, 4, 5, 6, 8, 8, 8, 8, 8 },
    { 8, 0, 4, 5, 6, 8, 8, 8, 8 },
    { 8, 1, 4, 5, 6, 8, 8, 8, 8 },
    { 8, 0, 1, 4, 5, 6, 8, 8, 8 },
    { 8, 2, 4, 5, 6, 8, 8, 8, 8 },
    { 8, 0, 2, 4, 5, 6, 8, 8, 8 },
    { 8, 1, 2, 4, 5, 6, 8, 8, 8 },
    { 8, 0, 1, 2, 4, 5, 6, 8, 8 },
    { 8, 3, 4, 5, 6, 8, 8, 8, 8 },
    { 8, 0, 3, 4, 5, 6, 8, 8, 8 },
    { 8, 1, 3, 4, 5, 6, 8, 8, 8 },
    { 8, 0, 1, 3, 4, 5, 6, 8, 8 },
    { 8, 2, 3, 4, 5, 6, 8, 8, 8 },
    { 8, 0, 2, 3, 4, 5, 6, 8, 8 },
    { 8, 1, 2, 3, 4, 5, 6, 8, 8 },
    { 8, 0, 1, 2, 3, 4, 5, 6, 8 },
    { 8, 7, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 1, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 7, 8, 8, 8, 8, 8 },
    { 8, 2, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 7, 8, 8, 8, 8, 8 },
    { 8, 1, 2, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 7, 8, 8, 8, 8 },
    { 8, 3, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 7, 8, 8, 8, 8, 8 },
    { 8, 1, 3, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 7, 8, 8, 8, 8 },
    { 8, 2, 3, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 7, 8, 8, 8, 8 },
    { 8, 1, 2, 3, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 7, 8, 8, 8 },
    { 8, 4, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 4, 7, 8, 8, 8, 8, 8 },
    { 8, 1, 4, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 4, 7, 8, 8, 8, 8 },
    { 8, 2, 4, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 4, 7, 8, 8, 8, 8 },
    { 8, 1, 2, 4, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 4, 7, 8, 8, 8 },
    { 8, 3, 4, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 4, 7, 8, 8, 8, 8 },
    { 8, 1, 3, 4, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 4, 7, 8, 8, 8 },
    { 8, 2, 3, 4, 7, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 4, 7, 8, 8, 8 },
    { 8, 1, 2, 3, 4, 7, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 4, 7, 8, 8 },
    { 8, 5, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 5, 7, 8, 8, 8, 8, 8 },
    { 8, 1, 5, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 5, 7, 8, 8, 8, 8 },
    { 8, 2, 5, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 5, 7, 8, 8, 8, 8 },
    { 8, 1, 2, 5, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 5, 7, 8, 8, 8 },
    { 8, 3, 5, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 5, 7, 8, 8, 8, 8 },
    { 8, 1, 3, 5, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 5, 7, 8, 8, 8 },
    { 8, 2, 3, 5, 7, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 5, 7, 8, 8, 8 },
    { 8, 1, 2, 3, 5, 7, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 5, 7, 8, 8 },
    { 8, 4, 5, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 4, 5, 7, 8, 8, 8, 8 },
    { 8, 1, 4, 5, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 4, 5, 7, 8, 8, 8 },
    { 8, 2, 4, 5, 7, 8, 8, 8, 8 },
    { 8, 0, 2, 4, 5, 7, 8, 8, 8 },
    { 8, 1, 2, 4, 5, 7, 8, 8, 8 },
    { 8, 0, 1, 2, 4, 5, 7, 8, 8 },
    { 8, 3, 4, 5, 7, 8, 8, 8, 8 },
    { 8, 0, 3, 4, 5, 7, 8, 8, 8 },
    { 8, 1, 3, 4, 5, 7, 8, 8, 8 },
    { 8, 0, 1, 3, 4, 5, 7, 8, 8 },
    { 8, 2, 3, 4, 5, 7, 8, 8, 8 },
    { 8, 0, 2, 3, 4, 5, 7, 8, 8 },
    { 8, 1, 2, 3, 4, 5, 7, 8, 8 },
    { 8, 0, 1, 2, 3, 4, 5, 7, 8 },
    { 8, 6, 7, 8, 8, 8, 8, 8, 8 },
    { 8, 0, 6, 7, 8, 8, 8, 8, 8 },
    { 8, 1, 6, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 1, 6, 7, 8, 8, 8, 8 },
    { 8, 2, 6, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 2, 6, 7, 8, 8, 8, 8 },
    { 8, 1, 2, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 2, 6, 7, 8, 8, 8 },
    { 8, 3, 6, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 3, 6, 7, 8, 8, 8, 8 },
    { 8, 1, 3, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 3, 6, 7, 8, 8, 8 },
    { 8, 2, 3, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 2, 3, 6, 7, 8, 8, 8 },
    { 8, 1, 2, 3, 6, 7, 8, 8, 8 },
    { 8, 0, 1, 2, 3, 6, 7, 8, 8 },
    { 8, 4, 6, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 4, 6, 7, 8, 8, 8, 8 },
    { 8, 1, 4, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 4, 6, 7, 8, 8, 8 },
    { 8, 2, 4, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 2, 4, 6, 7, 8, 8, 8 },
    { 8, 1, 2, 4, 6, 7, 8, 8, 8 },
    { 8, 0, 1, 2, 4, 6, 7, 8, 8 },
    { 8, 3, 4, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 3, 4, 6, 7, 8, 8, 8 },
    { 8, 1, 3, 4, 6, 7, 8, 8, 8 },
    { 8, 0, 1, 3, 4, 6, 7, 8, 8 },
    { 8, 2, 3, 4, 6, 7, 8, 8, 8 },
    { 8, 0, 2, 3, 4, 6, 7, 8, 8 },
    { 8, 1, 2, 3, 4, 6, 7, 8, 8 },
    { 8, 0, 1, 2, 3, 4, 6, 7, 8 },
    { 8, 5, 6, 7, 8, 8, 8, 8, 8 },
    { 8, 0, 5, 6, 7, 8, 8, 8, 8 },
    { 8, 1, 5, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 1, 5, 6, 7, 8, 8, 8 },
    { 8, 2, 5, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 2, 5, 6, 7, 8, 8, 8 },
    { 8, 1, 2, 5, 6, 7, 8, 8, 8 },
    { 8, 0, 1, 2, 5, 6, 7, 8, 8 },
    { 8, 3, 5, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 3, 5, 6, 7, 8, 8, 8 },
    { 8, 1, 3, 5, 6, 7, 8, 8, 8 },
    { 8, 0, 1, 3, 5, 6, 7, 8, 8 },
    { 8, 2, 3, 5, 6, 7, 8, 8, 8 },
    { 8, 0, 2, 3, 5, 6, 7, 8, 8 },
    { 8, 1, 2, 3, 5, 6, 7, 8, 8 },
    { 8, 0, 1, 2, 3, 5, 6, 7, 8 },
    { 8, 4, 5, 6, 7, 8, 8, 8, 8 },
    { 8, 0, 4, 5, 6, 7, 8, 8, 8 },
    { 8, 1, 4, 5, 6, 7, 8, 8, 8 },
    { 8, 0, 1, 4, 5, 6, 7, 8, 8 },
    { 8, 2, 4, 5, 6, 7, 8, 8, 8 },
    { 8, 0, 2, 4, 5, 6, 7, 8, 8 },
    { 8, 1, 2, 4, 5, 6, 7, 8, 8 },
    { 8, 0, 1, 2, 4, 5, 6, 7, 8 },
    { 8, 3, 4, 5, 6, 7, 8, 8, 8 },
    { 8, 0, 3, 4, 5, 6, 7, 8, 8 },
    { 8, 1, 3, 4, 5, 6, 7, 8, 8 },
    { 8, 0, 1, 3, 4, 5, 6, 7, 8 },
    { 8, 2, 3, 4, 5, 6, 7, 8, 8 },
    { 8, 0, 2, 3, 4, 5, 6, 7, 8 },
    { 8, 1, 2, 3, 4, 5, 6, 7, 8 },
    { 8, 0, 1, 2, 3, 4, 5, 6, 7 } };

} // namespace utils
} // namespace mt_kahypar