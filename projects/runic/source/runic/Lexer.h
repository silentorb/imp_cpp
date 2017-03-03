#pragma once

#include "Text_Source.h"
#include "Position.h"
#include <memory>

namespace runic {

  template<typename Character>
  class Lexer {
      std::unique_ptr<Text_Source<Character>> source;
      Character character = 0;
      Position position;

  public:
      Lexer(std::unique_ptr<Text_Source<Character>> &source) :
        source(std::move(source)) {}

      Lexer(Text_Source<Character> *source) :
        source(source) {
        character = source->next_character();
      }

      const Position &get_position() const {
        return position;
      }

      const Character &get_character() const {
        return character;
      }

      const Character &next_character() {
        if (character == '\n') {
          position.newline();
        }
        else {
          position.step();
        }

        return character = source->next_character();
      }
  };
}