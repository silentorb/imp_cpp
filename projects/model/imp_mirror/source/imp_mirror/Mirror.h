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
#include <underworld/expressions/Instantiation.h>
#include "Element_Map.h"
#include "Integrity.h"
#include "Temporary_Interface_Manager.h"

namespace overworld {
  class Block_Expression;
}

namespace imp_mirror {

  class Mirror {
      overworld::Profession_Library &profession_library;
      Element_Map &element_map;
      overworld::Graph &graph;
      Temporary_Interface_Manager &temporary_interface_manager;

      overworld::Dungeon *get_dungeon(overworld::Expression &expression) {
        auto &profession = expression.get_node()->get_element().get_profession();
        auto &dungeon = cast<const overworld::Dungeon>(profession);
        return const_cast<overworld::Dungeon *> (&dungeon);
      }

      overworld::Dungeon_Interface *get_dungeon_interface(overworld::Expression &expression) {
        auto &profession = expression.get_node()->get_element().get_profession();
        auto &dungeon = cast<const overworld::Dungeon_Interface>(profession);
        return const_cast<overworld::Dungeon_Interface *> (&dungeon);
      }

      overworld::Dungeon &get_dungeon(overworld::Member &member) {
        auto &member_dungeon = static_cast<overworld::Member_Dungeon &>(member);
        return member_dungeon.get_dungeon();
      }

      overworld::Dungeon_Interface &get_possible_generic_dungeon(overworld::Dungeon &dungeon);
      void reflect_enchantments(const underworld::Enchantment_Array &input_enchantments,
                                overworld::Enchantment_Container &output_enchantments,
                                overworld::Scope &scope);
      void apply_node_assignment(overworld::Node &target, overworld::Node &value);
      overworld::Expression_Owner reflect_assignment(const underworld::Assignment &input_assignment,
                                                     overworld::Scope &scope);

      overworld::Expression_Owner reflect_literal(const underworld::Literal &input_literal,
                                                  overworld::Dungeon &dungeon, overworld::Function_Interface *function);
      overworld::Expression_Owner reflect_member(const underworld::Member_Expression &input_member_expression,
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

//      overworld::Expression_Owner reflect_unresolved(overworld::Expression &previous,
//                                                     const underworld::Unresolved_Member_Expression &member_expression,
//                                                     overworld::Scope &scope);

      overworld::Expression_Owner reflect_statement_expression(const underworld::Expression &input_expression,
                                                               overworld::Scope &scope);
      overworld::Expression_Owner reflect_statement(const underworld::Expression &input_expression,
                                                    overworld::Scope &scope);
      overworld::Profession &reflect_profession(const underworld::Profession *profession,
                                                overworld::Scope &scope);
      overworld::Profession &reflect_profession(const underworld::Profession &profession,
                                                overworld::Scope &scope);

      overworld::Profession &reflect_profession_child(overworld::Member &member,
                                                      const underworld::Profession &profession);

      overworld::Profession &reflect_dungeon_reference(const underworld::Profession &profession,
                                                       overworld::Scope &scope);

      overworld::Profession &reflect_dungeon_usage(const underworld::Token_Profession &profession,
                                                   overworld::Scope &scope);

      void reflect_minion(const underworld::Minion &input_minion, overworld::Scope &output_scope);
      std::unique_ptr<overworld::Parameter> create_parameter(const underworld::Minion &input_minion, overworld::Scope &scope,
                                                                   overworld::Function_Interface &function);
      std::unique_ptr<overworld::Minion> create_minion(const underworld::Minion &input_minion,
                                                       overworld::Scope &scope);
      void reflect_function1(const underworld::Member &member, overworld::Scope &scope);
      void reflect_function2(const underworld::Function &input_function);
      void reflect_function3(const underworld::Function &input_function);
      void reflect_scope1(const underworld::Scope &input_scope, overworld::Scope &output_scope);
      void reflect_scope2(const underworld::Scope &input_scope, overworld::Scope &output_scope);
      void reflect_scope3(const underworld::Scope &input_scope, overworld::Scope &output_scope);
      void reflect_dungeon1(const underworld:: Dungeon & input_dungeon, overworld::Scope &output_scope);
      overworld::Profession &reflect_primitive(const underworld::Primitive &primitive);
      overworld::Expression_Owner reflect_invoke(const underworld::Invoke &function_call,
                                                 overworld::Scope &scope);
      overworld::Expression_Owner reflect_instantiation(const underworld::Instantiation &instantiation,
                                                        overworld::Scope &scope);
      overworld::Function_Signature &
      get_function_signature(overworld::Expression &expression, std::vector<overworld::Expression_Owner> &arguments,
                             overworld::Scope &scope);

      template<typename Output, typename Input>
      inline const Output &cast(const Input &expression) {
        return *dynamic_cast<const Output *>(&expression);
      }

      template<typename Output, typename Input>
      inline Output &cast(Input &expression) {
        return *dynamic_cast<Output *>(&expression);
      }

//      void reflect_dungeon1(const underworld::Dungeon &input, overworld::Dungeon &output);
//      void reflect_dungeon2(const underworld::Dungeon &input, overworld::Dungeon &output);

  public:
      Mirror(overworld::Profession_Library &profession_library, Element_Map &element_map, overworld::Graph &graph,
             Temporary_Interface_Manager &temporary_interface_manager) :
        profession_library(profession_library), element_map(element_map), graph(graph),
        temporary_interface_manager(temporary_interface_manager) {}

      void reflect_root(const underworld::Dungeon &input, overworld::Dungeon &output);
  };
}