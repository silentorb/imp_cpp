#pragma once

#include <string>
#include "Range.h"
#include "Match.h"

namespace runic {

  template<typename Whisper>
  class Rune {
      Match<Whisper> match;
      Range range;

  public:
      Rune() {}

      Match<Whisper> &get_match() {
        return match;
      }

      void set_match(const Match<Whisper> &value) {
        match = value;
      }

      Range &get_range() {
        return range;
      }

      void set_range(const Range &value) {
        range = value;
      }
  };

}