#pragma once

#include <string>
#include "Range.h"
#include "Match.h"

namespace runic {

  template<typename Whisper>
  class Rune {
      Generic_Match<Whisper> match;
      Range range;

  public:
//      Rune(Source *source) : source(source) {}

      Generic_Match<Whisper> &get_match() {
        return match;
      }

      const Generic_Match<Whisper> &get_match() const {
        return match;
      }

      void set_match(const Generic_Match<Whisper> &value) {
        match = value;
      }

      Range &get_range() {
        return range;
      }

      const Range &get_range() const {
        return range;
      }

      void set_range(const Range &value) {
        range = value;
      }

      inline bool is(const Whisper &whisper) const {
        return match.get_type() == &whisper;
      }

      inline bool is_not(const Whisper &whisper) const {
        return match.get_type() != &whisper;
      }

      const std::string &get_text() const {
        return match.get_text();
      }
  };

}