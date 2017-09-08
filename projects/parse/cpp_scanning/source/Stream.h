#pragma once

#include <runic_cpp/Whisper.h>
#include <runic_cpp/Token.h>
#include <runic_cpp/Cpp_Lexer.h>
#include <runic/Rune_Stream.h>
#include <source_mapping/Source_Point.h>
#include "exceptions.h"
#include <iostream>

namespace cpp_scanning {

  using Token = runic_cpp::Token;

  class Stream : public runic::Rune_Stream<runic_cpp::cpp_Lexer, Token, runic_cpp::Whisper> {
      source_mapping::Source_File &source_file;

  public:
      Stream(runic_cpp::cpp_Lexer &lexer, source_mapping::Source_File &source_file) :
        Rune_Stream(lexer), source_file(source_file) {}

      ~Stream() {}

      const Token &expect_next(const runic_cpp::Whisper &whisper) {
        if (!next().is(whisper))
          throw Expected_Whisper_Exception(current(), whisper);

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
  };
}