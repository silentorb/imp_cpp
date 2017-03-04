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

      const Match<Whisper> &get_match() const{
        return match;
      }

      void set_match(const Match<Whisper> &value) {
        match = value;
      }

      Range &get_range() {
        return range;
      }

      const Range &get_range() const{
        return range;
      }

      void set_range(const Range &value) {
        range = value;
      }

      inline bool is(const Whisper &whisper) const {
        return match.get_type() == &whisper;
      }

      const std::string &get_text() const {
        return match.get_text();
      }
  };

}