#pragma once

#include <underworld/expressions/Block.h>
#include "Base_Summoner.h"
#include "Context.h"

namespace underworld {
  class Member_Expression;
}

namespace imp_summoning {

  using Expression_Owner = underworld::Expression_Owner;

  class Expression_Summoner : public Base_Summoner {
      Expression_Owner process_variable_declaration(Context &context);
      Expression_Owner process_if(Context &context);
      Expression_Owner process_condition(Context &context);
      Expression_Owner process_statement(Context &context);
      Expression_Owner process_child(Expression_Owner &expression, Context &context);
      Expression_Owner identify_root(Context &context);
      Expression_Owner process_path(Context &context);
      Expression_Owner process_function_call(Expression_Owner &expression, Context &context);
      Expression_Owner process_instantiation(Expression_Owner profession_expression, Context &context);
      underworld::Operator_Type process_assignment_operator(Context &context);
      underworld::Operator_Type process_expression_operator(Context &context);
      Expression_Owner process_identifier(Context &context);
      Expression_Owner process_lambda(underworld::Member_Expression &parameter_info, Context &context);

  public:
      Expression_Summoner(runic::Stream &input, Parser_Lookup &lookup);

      void process_block(underworld::Block &block, Context &context);
      void process_expression_block(underworld::Block &block, Context &context);
      Expression_Owner process_block_or_single_expression(Context &context);
      Expression_Owner process_expression(Context &context);
  };
}