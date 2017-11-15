#pragma once

#include <stdexcept>
#include "Range.h"

namespace runic {

  template<typename Token>
  class Token_Exception : public std::runtime_error {
      const Token token;

  public:
      Token_Exception(const Token &token, const std::string &__arg) :
        token(token), runtime_error(__arg) {}

  };

  template<typename Token>
  class End_Of_File_Exception : public Token_Exception<Token> {
  public:
      End_Of_File_Exception(const Token &token) :
        Token_Exception<Token>(token, "Unexpected end of file.") {}
  };

//  template<typename Token, typename Whisper>
//  class Expected_Exception : public Token_Exception<Token> {
//      const Whisper &whisper;
//
//  public:
//      Expected_Exception(const Token &token, const Whisper &whisper) :
//        Token_Exception<Token>(token, "Unexpected end of file."), whisper(whisper) {}
//  };
}