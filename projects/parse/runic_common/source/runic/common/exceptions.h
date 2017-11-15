#pragma once

#include <stdexcept>
#include "Token.h"

namespace runic {

  class Invalid_Token_Exception : public Token_Exception<Token> {
      const Token token;

  public:
      Invalid_Token_Exception(const Token &token, const std::string &__arg) :
        Token_Exception(token, __arg + " at line " + token.get_start().get_string()) {}

  };

  class Syntax_Exception : public Token_Exception<Token> {
      inline static const std::string format_message(const Token &token, const std::string &filename) {
        auto info = token.get_info_string();
        auto position = token.get_start().get_string() + " of " + filename;
        return info != ""
               ? "Unexpected token \"" + info + "\" at line " + position
               : "Syntax Exception at line " + position;
      }

  public:
      Syntax_Exception(const Token &token, const std::string &filename) :
        Token_Exception(token, format_message(token, filename)) {}
  };

  class Expected_Exception : public Token_Exception<Token> {
      inline static const std::string format_message(const Token &token, const std::string &expected,
                                                     const std::string &filename) {
        auto info = token.get_info_string();
        auto first_part = "Expected " + expected + " at line " + token.get_start().get_string() + " of " + filename;
        return info != ""
               ? first_part + " but found \"" + info + "\"."
               : first_part + ".";
      }

  public:
      Expected_Exception(const std::string &expected, const Token &token, const std::string &filename) :
        Token_Exception(token, format_message(token, expected, filename)) {}
  };

  class Expected_Whisper_Exception : public Expected_Exception {
  public:
      Expected_Whisper_Exception(const Token &token, const Whisper &whisper, const std::string &filename) :
        Expected_Exception("" + whisper.get_name() + "", token, filename) {}
  };

  class Range_Exception : public std::runtime_error {
      const source_mapping::Source_Range range;

  public:
      Range_Exception(const source_mapping::Source_Range &range, const std::string &__arg) :
        range(range), runtime_error(__arg + " at " + range.get_start().to_string()) {}
  };

}