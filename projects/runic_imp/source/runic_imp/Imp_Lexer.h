#pragma once

#include <runic/Lexer.h>
#include <runic/Rune.h>
#include "Whisper.h"
#include "Lexicon.h"
#include <vector>

namespace runic_imp {

  using Char = char;
  using Token = runic::Rune<Whisper>;
  using Match = runic::Match<Whisper>;

  class Imp_Lexer {
      runic::Lexer<Char> lexer;
      Lexicon &lexicon;

      bool match_whitespace(Match &result);
      bool match_non_whitespace(Match &result);
      bool match_identifier(Match &result);
      bool match_number(Match &result);
      bool match_string(Match &result);
      bool match_comment(Match &result);
      bool match_comment_or_division(Match &result);
      bool match_special_symbols(Match & result);
      bool match_any(Match &result);

  public:
      Imp_Lexer(std::unique_ptr<runic::Text_Source<Char>> &source);
      Imp_Lexer(runic::Text_Source<Char> *source);
      bool next_token(Token &token);
      void get_all_tokens(std::vector<Token> &tokens);

  };
}