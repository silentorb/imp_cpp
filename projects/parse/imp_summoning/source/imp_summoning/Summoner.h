#pragma once

#include <underworld/expressions/Expression.h>
#include "Base_Summoner.h"
#include "Context.h"
#include "Expression_Summoner.h"

namespace imp_summoning {

  struct Identifier {
      const std::string name;
      const source_mapping::Source_Point source_point;
  };

  class Summoner : public Base_Summoner {
      Expression_Summoner expression_summoner;

      underworld::Dungeon &process_dungeon(const Identifier &identifier, Context &context);
      void process_dungeon_member(Context &context);
      underworld::Function &process_function(const std::string &name, Context &context);
      void process_function_parameters(Context &context, underworld::Function &func);
      void process_root(Context &context);
      void process_root_identifier(const Identifier &identifier, Context &context);
      void process_member(const Identifier &identifier, Context &context, bool is_static);
      underworld::Minion &process_minion(const Identifier &identifier, Context &context);
      void process_minion_or_dungeon(const Identifier &identifier, Context &context);
      void process_dungeon_with_contracts(const Identifier &identifier, underworld::Profession_Owner first_contract,
                                          Context &context);
      underworld::Profession_Owner process_optional_profession(Context &context);
//      underworld::Profession *find_profession(Context &context);
//      underworld::Profession *find_profession();

  public:
      Summoner(runic::Stream &lexer, Parser_Lookup &lookup);
      void summon(underworld::Dungeon &root);
  };
}