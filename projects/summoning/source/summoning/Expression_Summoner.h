#pragma once

#include <underworld/expressions/Block.h>
#include "Base_Summoner.h"
#include "Context.h"

namespace summoning {

  class Expression_Summoner : public Base_Summoner{
      void process_statements(underworld::Block &block, Context &context);
      void process_variable_declaration(Context &context);

  public:
      Expression_Summoner(Stream & input, underworld::Profession_Library &profession_library);
      underworld::Expression_Owner process_expression(Context & context);
  };
}