#pragma once

#include "Text_Source.h"
#include <memory>

namespace runic {

  template<typename Character>
  class Lexer {
      std::unique_ptr<Text_Source<Character>> source;
      unsigned long index = 0;

  public:
      Lexer(std::unique_ptr<Text_Source<Character>> &source) :
        source(std::move(source)) {}

      unsigned long get_index() const {
        return index;
      }

      Character next_character(){
        ++index;
        return source->next_character();
      }
  };
}