#pragma once

#include <stdexcept>
#include <runic_imp/Token.h>

namespace summoning {

  class Token_Exception : public std::runtime_error {
      const runic_imp::Token token;

  public:
      Token_Exception(const runic_imp::Token &token, const std::string &__arg) :
        token(token), runtime_error(__arg) {}

  };

  class Syntax_Exception : public Token_Exception {
      inline static const std::string format_message(const runic_imp::Token &token) {
        auto info = token.get_info_string();
        return info != ""
               ? "Unexpected token \"" + info + "\" at line " + token.get_start().get_string()
               : "Syntax Exception at line " + token.get_start().get_string();
      }

  public:
      Syntax_Exception(runic_imp::Token &token) :
        Token_Exception(token, format_message(token)) {}
  };

  class End_Of_File_Exception : public Token_Exception {
  public:
      End_Of_File_Exception(const Token &token) :
        Token_Exception(token, "Unexpected end of file at line " + token.get_start().get_string()) {}
  };

  class Expected_Exception : public Token_Exception {
      inline static const std::string format_message(const runic_imp::Token &token, const std::string &expected) {
        auto info = token.get_info_string();
        auto first_part = "Expected " + expected + " at line " + token.get_start().get_string();
        return info != ""
               ? first_part + " but found \"" + info + "\"."
               : first_part + ".";
      }

  public:
      Expected_Exception(const std::string &expected, runic_imp::Token &token) :
        Token_Exception(token, format_message(token, expected)) {}
  };

  class Expected_Whisper_Exception : public Expected_Exception {
  public:
      Expected_Whisper_Exception(const runic_imp::Whisper &whisper, Token &token) :
        Expected_Exception("" + whisper.get_name() + "", token) {}
  };
}