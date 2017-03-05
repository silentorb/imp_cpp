#pragma once

#include <underworld/expressions/Expression.h>
#include "Base_Summoner.h"
#include "Context.h"

namespace summoning {

  class Summoner : public Base_Summoner {

      void process_dungeon(const std::string &name, Context &context);
      void process_dungeon_member(Context &context);
      underworld::Expression_Owner process_expression(Context &context);
      void process_minion(const std::string &name, Context &context);
      void process_minion_parameters(underworld::Minion &minion, Context &context);
      void process_root(Context &context);
      void process_root_identifier(const std::string &name, Context &context);

  public:

      Summoner(Stream &lexer, underworld::Profession_Library &profession_library);
      void summon(underworld::Dungeon &root);
  };
}