#pragma once

#include <runic/Lexer.h>
#include <runic/Rune.h>
#include "Whisper.h"
#include "Common_Lexicon.h"
#include "Token.h"
#include <vector>

namespace runic {

  using Char = char;
  using Match = runic::Generic_Match<runic::Whisper>;

  enum class Token_Result {
      finished,
      token,
      loop,
      error, // Not yet used but might be someday.
  };

  class Common_Lexer {
      runic::Lexer<Char> lexer;
      runic::Common_Lexicon &lexicon;

      void consume_whitespace(bool &follows_whitespace);
      Token_Result match_non_whitespace(Match &result);
      void match_identifier(Match &result);
      void match_number(Match &result);
      void match_string(Match &result);
      void consume_to_end_of_line();
      Token_Result match_comment_or_division(Match &result);

      bool match_special_symbols(Match &result);
      bool match_any(Match &result, runic::Token &token, bool &follows_whitespace);

  public:
      Common_Lexer(std::unique_ptr<runic::Text_Source<Char>> source);
//      Common_Lexer(runic::Text_Source<Char> *source);
      bool next_token(runic::Token &token);
      void get_all_tokens(std::vector<runic::Token> &tokens);

      const runic::Position &get_position() const {
        return lexer.get_position();
      }

  };
}