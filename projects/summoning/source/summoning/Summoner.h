#pragma once

#include <runic_imp/Imp_Lexer.h>
#include <underworld/Underlord.h>
#include <underworld/schema/Profession_Library.h>
#include <underworld/expressions/Block.h>
#include "Context.h"

namespace summoning {

  using Token = runic_imp::Token;
  using Underlord = underworld::Underlord;

  class Summoner {
      runic_imp::Imp_Lexer &lexer;
      const runic_imp::Symbols &lexicon;
      underworld::Profession_Library &profession_library;

      bool get_next(Token &token) {
        return lexer.next_token(token);
      }

      bool require_next(Token &token, const runic_imp::Whisper &whisper);
      void expect(Token &token, const runic_imp::Whisper &whisper);

      void process_variable_declaration(Context &context);
      void process_dungeon_member(Token &token, Context &context);
      void process_root_identifier(const std::string &name, Context &context);
      void process_minion_parameters(underworld::Minion &minion, Context &context);
      void process_minion(const std::string &name, Context &context);
      void process_statements(underworld::Block &block, Context &context);
      void process_dungeon(const std::string &name, Context &context);
      void process_root(Context &context);

  public:
      Summoner(runic_imp::Imp_Lexer &lexer, underworld::Profession_Library &profession_library);
      void summon(underworld::Dungeon &root);
  };
}