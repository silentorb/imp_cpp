#pragma once

#include <runic_imp/Whisper.h>
#include <runic_imp/Token.h>
#include <runic_imp/Imp_Lexer.h>
#include <runic/Rune_Stream.h>
#include "exceptions.h"

namespace imp_summoning {

  using Token = runic_imp::Token;

  class Stream : public runic::Rune_Stream<runic_imp::Imp_Lexer, Token, runic_imp::Whisper> {
  public:
      Stream(runic_imp::Imp_Lexer &lexer) : Rune_Stream(lexer) {}

      const Token &expect_next(const runic_imp::Whisper &whisper) {
        if (!next().is(whisper))
          throw Expected_Whisper_Exception(current(), whisper);

        return current();
      }
  };
}