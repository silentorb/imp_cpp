#pragma once

#include <overworld/schema/Dungeon.h>
#include <underworld/schema/Dungeon.h>
#include <overworld/schema/Profession_Library.h>
#include <overworld/expressions/Expression.h>
#include <overworld/expressions/Operator.h>
#include <underworld/expressions/Block.h>
#include <underworld/expressions/Flow_Control.h>
#include <underworld/expressions/Literal.h>
#include <underworld/expressions/Return.h>
#include <underworld/expressions/Minion_Declaration.h>
#include <underworld/expressions/Minion_Expression.h>
#include <underworld/expressions/Operator.h>
#include <underworld/expressions/Assignment.h>
#include "Element_Map.h"
#include "Integrity.h"

namespace overworld {
  class Block;
}

namespace imp_mirror {

  class Mirror {
      overworld::Profession_Library &profession_library;
      Element_Map &element_map;
      Integrity integrity;

      overworld::Expression_Owner reflect_assignment(const underworld::Assignment &input_assignment,
                                                     overworld::Scope &scope);
      void reflect_scope(const underworld::Scope &input_scope, overworld::Scope &output_scope);
      overworld::Expression_Owner reflect_literal(const underworld::Literal &input_literal);
      overworld::Expression_Owner reflect_minion(const underworld::Minion_Expression &input_minion_expression,
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
      overworld::Expression_Owner reflect_block(const underworld::Block &input_block);
      void reflect_block(const underworld::Block &input_block, overworld::Block &output_block);
      overworld::Expression_Owner reflect_expression(const underworld::Expression &input_expression,
                                                     overworld::Scope &scope);
      overworld::Expression_Owner reflect_statement(const underworld::Expression &input_expression,
                                                    overworld::Scope &scope);
      const overworld::Profession &reflect_profession(const underworld::Profession &profession);

  public:
      Mirror(overworld::Profession_Library &profession_library, Element_Map &element_map) :
        profession_library(profession_library), element_map(element_map) {}

      void reflect_dungeon(const underworld::Dungeon &input, overworld::Dungeon &output);
  };
}