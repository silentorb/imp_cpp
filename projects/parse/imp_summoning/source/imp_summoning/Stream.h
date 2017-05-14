#pragma once

#include <runic_imp/Whisper.h>
#include <runic_imp/Token.h>
#include <runic_imp/Imp_Lexer.h>
#include <runic/Rune_Stream.h>
#include <underworld/Source_Point.h>
#include "exceptions.h"

namespace imp_summoning {

  using Token = runic_imp::Token;

  class Stream : public runic::Rune_Stream<runic_imp::Imp_Lexer, Token, runic_imp::Whisper> {
      underworld::Source_File source_file;

  public:
      Stream(runic_imp::Imp_Lexer &lexer, underworld::Source_File &source_file) :
        Rune_Stream(lexer), source_file(source_file) {}

      const Token &expect_next(const runic_imp::Whisper &whisper) {
        if (!next().is(whisper))
          throw Expected_Whisper_Exception(current(), whisper);

        return current();
      }

      const underworld::Source_File &get_source_file() const {
        return source_file;
      }

      const underworld::Source_Point get_source_point() const {
        return underworld::Source_Point(source_file, get_position().get_row(), get_position().get_column());
      }
  };
}