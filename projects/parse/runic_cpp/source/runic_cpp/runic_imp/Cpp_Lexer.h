#pragma once

#include <runic/Lexer.h>
#include <runic/Rune.h>
#include "Whisper.h"
#include "Lexicon.h"
#include "Token.h"
#include <vector>

namespace runic_cpp {

  using Char = char;
  using Match = runic::Match<Whisper>;

  enum class Token_Result {
      finished,
      token,
      loop,
      error, // Not yet used but might be someday.
  };

  class Cpp_Lexer {
      runic::Lexer<Char> lexer;
      Lexicon &lexicon;
      bool follows_whitespace = false;

      void consume_whitespace();
      Token_Result match_non_whitespace(Match &result);
      void match_identifier(Match &result);
      void match_number(Match &result);
      void match_string(Match &result);
      void consume_to_end_of_line();
      Token_Result match_comment_or_division(Match &result);

      bool match_special_symbols(Match &result);
      bool match_any(Match &result, Token &token);

  public:
      Cpp_Lexer(std::unique_ptr<runic::Text_Source<Char>> &source);
      Cpp_Lexer(runic::Text_Source<Char> *source);
      bool next_token(Token &token);
      void get_all_tokens(std::vector<Token> &tokens);

      const runic::Position &get_position() const {
        return lexer.get_position();
      }

  };
}