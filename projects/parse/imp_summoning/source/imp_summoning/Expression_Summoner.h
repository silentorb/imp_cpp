#pragma once

#include <underworld/expressions/Block.h>
#include "Base_Summoner.h"
#include "Context.h"

namespace summoning {

  using Expression_Owner = underworld::Expression_Owner;

  class Expression_Summoner : public Base_Summoner {
      Expression_Owner process_variable_declaration(Context &context);
      Expression_Owner process_if(Context &context);
      Expression_Owner process_condition(Context &context);
      Expression_Owner process_statement(Context &context);

  public:
      Expression_Summoner(Stream &input, underworld::Profession_Library &profession_library);

      void process_block(underworld::Block &block, Context &context);
      Expression_Owner process_block_or_single_expression(Context &context);
      Expression_Owner process_expression(Context &context);
  };
}