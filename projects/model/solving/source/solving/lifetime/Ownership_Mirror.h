#pragma once

#include <overworld/expressions/Expression.h>
#include <overworld/schema/Function.h>
#include <overworld/expressions/Instantiation.h>
#include <overworld/expressions/Assignment.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/expressions/Chain.h>
#include "Graph.h"

namespace lifetime {
//  enum class Expression_Type {
//      assignment,
//      block,
//      chain,
//      Else,
//      If,
//      instantiation,
//      invoke,
//      lambda,
//      literal,
//      member,
//      Operator, // Infix
//      range,
//      return_nothing,
//      return_with_value,
//      self,
//      variable_declaration,
//      variable_declaration_and_assignment
//  };

  class Ownership_Mirror {
      Graph &graph;
      Node &create_node(const Overworld_Element &profession_source, Lifetime_Ownership ownership);
      Node &reflect_instantiation(overworld::Instantiation &instantiation);
      Node &reflect_member(overworld::Member_Expression &member_expression);
      void reflect_assignment(overworld::Assignment &assignment);
//      void reflect_variable_declaration(overworld::Minion_Declaration &variable_declaration);
      Node &reflect_chain(overworld::Chain &chain);
      void reflect_variable_declaration_and_assignment(overworld::Minion_Declaration_And_Assignment
                                                       &declaration);
      Node &reflect_expression(overworld::Expression &expression);
      Node *reflect_expression_statement(overworld::Expression &expression);
      void reflect_statement(overworld::Expression &expression);

      void reflect_block(overworld::Block &block);
      void reflect_function(overworld::Function_With_Block &function);
      void reflect_dungeon(overworld::Dungeon &dungeon);

  public:
      explicit Ownership_Mirror(Graph &graph) : graph(graph) {}

      void reflect(overworld::Dungeon &overworld_root);
  };
}