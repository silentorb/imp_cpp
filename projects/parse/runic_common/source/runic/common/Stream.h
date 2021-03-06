#pragma once

#include <runic/Rune_Stream.h>
#include <source_mapping/Source_Point.h>
#include "Token.h"
#include "Common_Lexer.h"
#include "exceptions.h"
#include <iostream>

namespace runic {

  class Stream : public runic::Rune_Stream<Lexer_Interface, Token, Whisper> {
      source_mapping::Source_File &source_file;

  public:
      Stream(Lexer_Interface &lexer, source_mapping::Source_File &source_file) :
        Rune_Stream(lexer), source_file(source_file) {}

      virtual ~Stream() {}

      const Token &expect_next(const Whisper &whisper) {
        if (!next().is(whisper))
          throw Expected_Whisper_Exception(current(), whisper, source_file.get_file_path());

        return current();
      }

      const Token &expect(const Whisper &whisper) {
        if (!current().is(whisper))
          throw Expected_Whisper_Exception(current(), whisper, source_file.get_file_path());

        return current();
      }

      const source_mapping::Source_File &get_source_file() const {
        return source_file;
      }

      const source_mapping::Source_Point get_source_point() const {
        auto &position = current().get_range().get_start();
//        std::cout << current().get_text() << " " << position.get_row() << ":"<< position.get_column() << std::endl;
        return source_mapping::Source_Point(source_file, position.get_row(), position.get_column());
      }

      const source_mapping::Source_Range get_source_range() const {
        auto &range = current().get_range();
        return source_mapping::Source_Range(
          {source_file, range.get_start().get_row(), range.get_start().get_column()},
          {source_file, range.get_end().get_row(), range.get_end().get_column()}
        );
      }
  };
}