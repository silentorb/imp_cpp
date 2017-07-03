#pragma once

#include <overworld/schema/Dungeon.h>
#include <underworld/schema/Dungeon.h>
#include <overworld/schema/professions/Profession_Library.h>
#include <overworld/expressions/Expression.h>
#include <overworld/expressions/Operator.h>
#include <underworld/expressions/Block.h>
#include <underworld/expressions/Flow_Control.h>
#include <underworld/expressions/Literal.h>
#include <underworld/expressions/Return.h>
#include <underworld/expressions/Minion_Declaration.h>
#include <underworld/expressions/Member_Expression.h>
#include <underworld/expressions/Operator.h>
#include <underworld/expressions/Assignment.h>
#include <overworld/imp_graph/Graph.h>
#include <overworld/expressions/Invoke.h>
#include <underworld/expressions/Chain.h>
#include "Element_Map.h"
#include "Integrity.h"

namespace overworld {
  class Block_Expression;
}

namespace imp_mirror {

  class Mirror {
      overworld::Profession_Library &profession_library;
      Element_Map &element_map;
      overworld::Graph &graph;
      Integrity integrity;

      overworld::Dungeon *get_dungeon(overworld::Expression &expression) {
        auto &profession = expression.get_node()->get_profession_reference().get_profession();
        auto &dungeon = cast<const overworld::Dungeon>(profession);
        return const_cast<overworld::Dungeon *> (&dungeon);
      }

      overworld::Expression_Owner reflect_assignment(const underworld::Assignment &input_assignment,
                                                     overworld::Scope &scope);
      void reflect_scope1(const underworld::Scope &input_scope, overworld::Scope &output_scope);
      void reflect_scope2(const underworld::Scope &input_scope, overworld::Scope &output_scope);
      overworld::Expression_Owner reflect_literal(const underworld::Literal &input_literal);
      overworld::Expression_Owner reflect_method(const underworld::Member_Expression &input_member_expression,
                                                 overworld::Scope &scope);
      overworld::Operator_Type reflect_operator(const underworld::Operator &input_operator);
      overworld::Expression_Owner reflect_return_nothing(const underworld::Return &input_return);
      overworld::Expression_Owner reflect_return_with_value(const underworld::Return_With_Value &input_return,
                                                            overworld::Scope &scope);
      overworld::Expression_Owner reflect_variable_declaration(const underworld::Minion_Declaration &input_declaration,
                                                               overworld::Scope &scope);
      overworld::Expression_Owner reflect_variable_declaration_with_assignment(
        const underworld::Minion_Declaration_And_Assignment &input_declaration, overworld::Scope &scope);


      overworld::Expression_Owner reflect_if(const underworld::If &input_if,
                                             overworld::Scope &scope);
      overworld::Expression_Owner reflect_block_expression(const underworld::Block &input_block,
                                                           overworld::Scope &scope);
      void reflect_block(const underworld::Block &input_block, overworld::Block &output_block);

      overworld::Expression_Owner reflect_expression(const underworld::Expression &input_expression,
                                                     overworld::Scope &scope);

      overworld::Expression_Owner reflect_chain(const underworld::Chain &input_chain,
                                                overworld::Scope &scope);

      overworld::Expression_Owner reflect_chain_member(overworld::Expression &first,
                                                       const underworld::Expression &second,
                                                       overworld::Scope &scope);

      overworld::Expression_Owner reflect_unresolved(overworld::Expression &previous,
                                                     const underworld::Unresolved_Member_Expression &member_expression,
                                                     overworld::Scope &scope);

      overworld::Expression_Owner reflect_statement_expression(const underworld::Expression &input_expression,
                                                               overworld::Scope &scope);
      overworld::Expression_Owner reflect_statement(const underworld::Expression &input_expression,
                                                    overworld::Scope &scope);
      const overworld::Profession &reflect_profession(const underworld::Profession &profession);
      void reflect_function1(const underworld::Member &member, overworld::Scope &scope);
      void reflect_function2(const underworld::Function &input_function);

      const overworld::Profession &reflect_primitive(const underworld::Primitive &primitive);
      overworld::Expression_Owner reflect_function_call(const underworld::Invoke &function_call,
                                                        overworld::Scope &scope);
      overworld::Function &get_function(overworld::Expression &expression);

      template<typename Output, typename Input>
      inline const Output &cast(const Input &expression) {
        return *dynamic_cast<const Output *>(&expression);
      }

//      void reflect_dungeon1(const underworld::Dungeon &input, overworld::Dungeon &output);
//      void reflect_dungeon2(const underworld::Dungeon &input, overworld::Dungeon &output);

  public:
      Mirror(overworld::Profession_Library &profession_library, Element_Map &element_map, overworld::Graph &graph) :
        profession_library(profession_library), element_map(element_map), graph(graph) {}

      void reflect_dungeon(const underworld::Dungeon &input, overworld::Dungeon &output);
  };
}