#pragma once

#include <list>
#include "exceptions.h"
#include "Position.h"

namespace runic {

  template<typename Lexer, typename Rune, typename Whisper>
  class Rune_Stream {
      Lexer &lexer;
      std::list<Rune> buffer;

      Rune &at(int index) {
        int i = 0;
        for (auto &entry : buffer) {
          if (i++ == index)
            return entry;
        }

        throw std::runtime_error("Requested index larger than buffer.");
      }

  public:
      Rune_Stream(Lexer &lexer) : lexer(lexer), buffer(1) {}

      const Rune &next() {
        if (buffer.size() > 1) {
          buffer.pop_front();
        }
        else {
          buffer.resize(1);
          lexer.next_token(buffer.front());
        }

        return buffer.front();
      }

      const Rune &peek() {
        if (buffer.size() == 1) {
          buffer.emplace_back();
          auto &entry = at(1);
          lexer.next_token(entry);
          return entry;
        }
        else {
          return at(1);
        }
      }

      const Rune &current() const {
        return buffer.front();
      }

      inline bool if_is(const Whisper &whisper) {
        if (current().is(whisper)) {
          next();
          return true;
        }

        return false;
      }

      bool until(const Whisper &whisper) {
        if (!peek().is(whisper)) {
          return true;
        }
        else {
          next();
          return false;
        }
      }

      const Position &get_position() const {
        return lexer.get_position();
      }

  public:

  };
}