#pragma once

#include "Position.h"

namespace runic {

  class Range {
      Position start;
      Position end;

  public:
      Range() {}

      Range(const Position &start, const Position &end) :
        start(start), end(end) {}

      const Position &get_start() const {
        return start;
      }

      const Position &get_end() const {
        return end;
      }

      void set_start(const Position &value) {
        start = value;
      }

      void set_end(const Position &value) {
        end = value;
      }
  };
}