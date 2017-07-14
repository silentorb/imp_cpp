#pragma once

#include <runic/Lexer.h>
#include <runic/Rune.h>
#include "Whisper.h"
#include "Lexicon.h"
#include "Token.h"
#include <vector>

namespace runic_imp {

  using Char = char;
  using Match = runic::Match<Whisper>;

//  enum class Match_Result {
//      success,
//      end_of_file
//  };

  class Imp_Lexer {
      runic::Lexer<Char> lexer;
      Lexicon &lexicon;
      bool follows_whitespace = false;

      void consume_whitespace();
      void match_non_whitespace(Match &result);
      void match_identifier(Match &result);
      void match_number(Match &result);
      void match_string(Match &result);
      void consumer_to_end_of_line();
      void match_comment_or_division(Match &result);

      bool match_special_symbols(Match &result);
      bool match_any(Match &result, Token &token);

  public:
      Imp_Lexer(std::unique_ptr<runic::Text_Source<Char>> &source);
      Imp_Lexer(runic::Text_Source<Char> *source);
      bool next_token(Token &token);
      void get_all_tokens(std::vector<Token> &tokens);

      const runic::Position &get_position() const {
        return lexer.get_position();
      }

  };
}