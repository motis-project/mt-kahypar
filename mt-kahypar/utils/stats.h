/*******************************************************************************
 * MIT License
 *
 * This file is part of Mt-KaHyPar.
 *
 * Copyright (C) 2019 Tobias Heuer <tobias.heuer@kit.edu>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

#include "mt-kahypar/macros.h"

namespace mt_kahypar {
namespace utils {
class StatsT {
  static constexpr bool debug = false;

 public:
  enum class Type : uint8_t {
    BOOLEAN = 0,
    INT32 = 1,
    INT64 = 2,
    FLOAT = 3,
    DOUBLE = 4
  };

  class Stat {
   public:
    explicit Stat(const bool value) :
      _type(Type::BOOLEAN),
      _value_1(value),
      _value_2(0),
      _value_3(0),
      _value_4(0.0),
      _value_5(0.0) { }

    explicit Stat(const int32_t value) :
      _type(Type::INT32),
      _value_1(false),
      _value_2(value),
      _value_3(0),
      _value_4(0.0),
      _value_5(0.0) { }

    explicit Stat(const int64_t value) :
      _type(Type::INT64),
      _value_1(false),
      _value_2(0),
      _value_3(value),
      _value_4(0.0),
      _value_5(0.0) { }

    explicit Stat(const float value) :
      _type(Type::FLOAT),
      _value_1(false),
      _value_2(0),
      _value_3(0),
      _value_4(value),
      _value_5(0.0) { }

    explicit Stat(const double value) :
      _type(Type::DOUBLE),
      _value_1(false),
      _value_2(0),
      _value_3(0),
      _value_4(0.0),
      _value_5(value) { }

    template <typename T>
    void update(const T ) { }

    void update(const bool value) {
      _value_1 = value;
    }

    void update(const int32_t delta) {
      _value_2 += delta;
    }

    void update(const int64_t delta) {
      _value_3 += delta;
    }

    void update(const float delta) {
      _value_4 += delta;
    }

    void update(const double delta) {
      _value_5 += delta;
    }

    friend std::ostream & operator<< (std::ostream& str, const Stat& stat);

   private:
    Type _type;
    bool _value_1;
    int32_t _value_2;
    int64_t _value_3;
    float _value_4;
    double _value_5;
  };

 public:
  explicit StatsT() :
    _stat_mutex(),
    _stats(),
    _enable(true) { }

  StatsT(const StatsT& other) :
    _stat_mutex(),
    _stats(other._stats),
    _enable(other._enable) { }

  StatsT & operator= (const StatsT &) = delete;

  StatsT(StatsT&& other) :
    _stat_mutex(),
    _stats(std::move(other._stats)),
    _enable(std::move(other._enable)) { }

  StatsT & operator= (StatsT &&) = delete;

  void enable() {
    std::lock_guard<std::mutex> lock(_stat_mutex);
    _enable = true;
  }

  void disable() {
    std::lock_guard<std::mutex> lock(_stat_mutex);
    _enable = false;
  }
  template <typename T>
  void add_stat(const std::string& key, const T value) {
    std::lock_guard<std::mutex> lock(_stat_mutex);
    if (_enable) {
      if (_stats.find(key) == _stats.end()) {
        _stats.emplace(
          std::piecewise_construct,
          std::forward_as_tuple(key),
          std::forward_as_tuple(value));
      }
    }
  }

  template <typename T>
  void update_stat(const std::string& key, const T delta) {
    std::lock_guard<std::mutex> lock(_stat_mutex);
    if (_enable) {
      if (_stats.find(key) == _stats.end()) {
        _stats.emplace(
          std::piecewise_construct,
          std::forward_as_tuple(key),
          std::forward_as_tuple(delta));
      } else {
        _stats.at(key).update(delta);
      }
    }
  }

  void clear() {
    _stats.clear();
  }

  friend std::ostream & operator<< (std::ostream& str, const StatsT& stats);

 private:
  std::mutex _stat_mutex;
  std::unordered_map<std::string, Stat> _stats;
  bool _enable;
};

inline std::ostream & operator<< (std::ostream& str, const StatsT::Stat& stat) {
  switch (stat._type) {
    case StatsT::Type::BOOLEAN:
      str << std::boolalpha << stat._value_1;
      break;
    case StatsT::Type::INT32:
      str << stat._value_2;
      break;
    case StatsT::Type::INT64:
      str << stat._value_3;
      break;
    case StatsT::Type::FLOAT:
      str << stat._value_4;
      break;
    case StatsT::Type::DOUBLE:
      str << stat._value_5;
      break;
    default:
      break;  // UNKNOWN TYPE
  }
  return str;
}

inline std::ostream & operator<< (std::ostream& str, const StatsT& stats) {
  std::vector<std::string> keys;
  for (const auto& stat : stats._stats) {
    keys.emplace_back(stat.first);
  }
  std::sort(keys.begin(), keys.end());

  for (const std::string& key : keys) {
    str << " " << key << "=" << stats._stats.at(key);
  }
  return str;
}

class DoNothingStats {
 public:
  DoNothingStats(const DoNothingStats&) = delete;
  DoNothingStats & operator= (const DoNothingStats &) = delete;

  DoNothingStats(DoNothingStats&&) = delete;
  DoNothingStats & operator= (DoNothingStats &&) = delete;

  static DoNothingStats& instance() {
    static DoNothingStats instance;
    return instance;
  }

  void enable() { }
  void disable() { }

  template <typename T>
  void add_stat(const std::string&, const T) { }

  template <typename T>
  void update_stat(const std::string&, const T) { }

  friend std::ostream & operator<< (std::ostream& str, const DoNothingStats& stats);

 private:
  explicit DoNothingStats() { }
};

inline std::ostream & operator<< (std::ostream& str, const DoNothingStats&) {
  return str;
}

#ifdef MT_KAHYPAR_LIBRARY_MODE
using Stats = DoNothingStats;
#else
using Stats = StatsT;
#endif

}  // namespace utils
}  // namespace mt_kahypar
