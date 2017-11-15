#pragma once

#include <underworld/expressions/Block.h>
#include "Base_Summoner.h"
#include "Context.h"

namespace underworld {
  class Member_Expression;

  class Chain;
}

namespace imp_summoning {

  using Expression_Owner = underworld::Expression_Owner;

  class Expression_Summoner : public Base_Summoner {
      Expression_Owner process_variable_declaration(Context &context);
      Expression_Owner process_if(Context &context);
      Expression_Owner process_condition(Context &context);
      Expression_Owner process_statement(Context &context);
//      void process_child(Expression_Owner &expression, std::vector<Link> &path, Context &context);
//      void process_chain(std::vector<Link> &path);
      Expression_Owner identify_root(Context &context);
      void process_path(std::vector<Link> &path, Context &context);
      Expression_Owner process_function_call(Expression_Owner &expression, Context &context);
      Expression_Owner process_instantiation(underworld::Profession_Owner profession_expression, Context &context);
      underworld::Operator_Type process_assignment_operator(Context &context);
      underworld::Operator_Type process_expression_operator(Context &context);
      Expression_Owner process_identifier(Context &context);
      underworld::Profession_Owner path_to_profession(const std::vector<Link> &path, Context &context);
      Expression_Owner process_lambda(underworld::Member_Expression &parameter_info, Context &context);

  public:
      Expression_Summoner(runic::Stream &input, Parser_Lookup &lookup);

      void process_block(underworld::Block &block, Context &context);
      void process_expression_block(underworld::Block &block, Context &context);
      Expression_Owner process_block_or_single_expression(Context &context);
      Expression_Owner process_expression(Context &context);
  };
}