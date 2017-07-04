#pragma once

#include <underworld/expressions/Expression.h>
#include "Base_Summoner.h"
#include "Context.h"
#include "Expression_Summoner.h"

namespace imp_summoning {

  class Summoner : public Base_Summoner {
      Expression_Summoner expression_summoner;

      void process_dungeon(const std::string &name, Context &context);
      void process_dungeon_member(Context &context);
      void process_function(const std::string &name, Context &context);
      void process_function_parameters(Context &context, underworld::Function &func);
      void process_root(Context &context);
      void process_root_identifier(const std::string &name, Context &context);
      void process_minion(const std::string &name, Context &context);
      underworld::Profession_Owner process_optional_profession(Context &context);
       underworld::Profession_Owner process_profession(Context &context);
//      underworld::Profession *find_profession(Context &context);
//      underworld::Profession *find_profession();

  public:
      Summoner(Stream &lexer, Lookup &lookup);
      void summon(underworld::Dungeon &root);
  };
}