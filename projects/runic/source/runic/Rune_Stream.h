#pragma once

#include <list>
#include "exceptions.h"

namespace runic {

  template<typename Lexer, typename Rune, typename Whisper>
  class Rune_Stream {
      Lexer &lexer;
      std::list<Rune> buffer;
      Rune& at(int index) {
        return  *(buffer.begin() + 1);
      }

  public:
      Rune_Stream(Lexer &lexer) : lexer(lexer) {}

      const Rune &next() {
        lexer.next_token(buffer.front());
        return buffer.front();
      }

      const Rune &peek() {
        lexer.next_token(buffer.front() + 1);
        return buffer.front();
      }

      const Rune &current() const {
        return buffer.front();
      }
//      bool get_next(Rune &token) {
//        return lexer.next_token(token);
//      }



//      void require_next(Rune &token) {
//        if (!lexer.next_token(token))
//          throw End_Of_File_Exception<Rune>(token);
//      }

//      bool require_until(Rune &token, const Whisper &whisper) {
//        if (!lexer.next_token(token))
//          throw Expected_Whisper_Exception<Rune, Whisper>(token, whisper);
//
//        return !token.is(whisper);
//      }
//
//      void expect(Rune &token, const Whisper &whisper) {
//        if (!lexer.next_token(token) || !token.is(whisper))
//          throw Expected_Whisper_Exception<Rune, Whisper>(token, whisper);
//      }

  public:

  };
}