#pragma once

#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon_Reference.h>
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
#include <underworld/expressions/Lambda.h>
#include <imp_mirror/Scope.h>
#include <underworld/expressions/Range.h>
#include "Element_Map.h"
#include "Temporary_Interface_Manager.h"
#include "Connector.h"

namespace overworld {
  class Block_Expression;
}

namespace imp_mirror {

  class Mirror {
      overworld::Profession_Library &profession_library;
      Element_Map &element_map;
      overworld::Graph *graph;
      Temporary_Interface_Manager &temporary_interface_manager;
      overworld::File_Library &header_files;
      Connector connector;

      void connect(overworld::Node &first, overworld::Node &second);

      overworld::Basic_Dungeon *get_dungeon(overworld::Profession &profession) {
        auto dungeon_reference = dynamic_cast<const overworld::Dungeon_Reference *>(&profession);
        return const_cast<overworld::Basic_Dungeon *> (&dungeon_reference->get_dungeon());
      }

      overworld::Basic_Dungeon *get_dungeon_interface(overworld::Profession &profession) {
        auto dungeon_reference = dynamic_cast<const overworld::Dungeon_Reference *>(&profession);
        auto dungeon_interface = dynamic_cast<const overworld::Basic_Dungeon *>(&dungeon_reference->get_dungeon());
        return const_cast<overworld::Basic_Dungeon *> (dungeon_interface);
      }

      overworld::Dungeon *get_dungeon(overworld::Expression &expression) {
        auto &profession = expression.get_profession();
        auto dungeon_reference = dynamic_cast<const overworld::Dungeon_Reference *>(profession.get());
        return const_cast<overworld::Dungeon *> (&dungeon_reference->get_dungeon().get_original());
      }

      overworld::Basic_Dungeon *get_dungeon_interface(overworld::Expression &expression) {
        auto &profession = expression.get_profession();
        auto dungeon_reference = dynamic_cast<const overworld::Dungeon_Reference *>(profession.get());
        auto dungeon_interface = dynamic_cast<const overworld::Basic_Dungeon *>(&dungeon_reference->get_dungeon());
        return const_cast<overworld::Basic_Dungeon *> (dungeon_interface);
      }

      overworld::Dungeon &get_dungeon(overworld::Member &member) {
        return member.get_dungeon();
      }

      overworld::Profession_Reference get_possible_generic_dungeon(overworld::Dungeon &dungeon);

      overworld::Dungeon *find_enchantment_dungeon(const underworld::Profession &input_profession,
                                                   Scope &scope);
      void reflect_enchantments(const underworld::Enchantment_Array &input_enchantments,
                                overworld::Enchantment_Container &output_enchantments,
                                Scope &scope);
      void apply_node_assignment(overworld::Node &target, overworld::Node &value);
      overworld::Expression_Owner reflect_assignment(const underworld::Assignment &input_assignment,
                                                     Scope &scope);

      overworld::Expression_Owner reflect_lambda(const underworld::Lambda &input_lambda, Scope &scope);
      overworld::Expression_Owner reflect_literal(const underworld::Literal &input_literal, overworld::Parent parent);
      overworld::Expression_Owner reflect_member(const underworld::Member_Expression &input_member_expression,
                                                 Scope &scope);
      overworld::Operator_Type reflect_operator(const underworld::Operator &input_operator);
      overworld::Expression_Owner reflect_return_nothing(const underworld::Return &input_return);
      overworld::Expression_Owner reflect_return_with_value(const underworld::Return_With_Value &input_return,
                                                            Scope &scope);
      overworld::Expression_Owner reflect_variable_declaration(const underworld::Minion_Declaration &input_declaration,
                                                               Scope &scope);
      overworld::Expression_Owner reflect_variable_declaration_with_assignment(
        const underworld::Minion_Declaration_And_Assignment &input_declaration, Scope &scope);

      overworld::Expression_Owner reflect_if(const underworld::If &input_if,
                                             Scope &scope);
      overworld::Expression_Owner reflect_block_expression(const underworld::Block &input_block,
                                                           Scope &scope);
      void reflect_block(const underworld::Block &input_block, overworld::Block &output_block, Scope &scope);

      overworld::Expression_Owner reflect_expression(const underworld::Expression &input_expression,
                                                     Scope &scope);

      overworld::Expression_Owner reflect_chain(const underworld::Chain &input_chain,
                                                Scope &scope);

      overworld::Expression_Owner reflect_chain_member(overworld::Expression &first,
                                                       const underworld::Expression &second,
                                                       Scope &scope);

//      overworld::Expression_Owner reflect_unresolved(overworld::Expression &previous,
//                                                     const underworld::Unresolved_Member_Expression &member_expression,
//                                                     Scope &scope);

      overworld::Expression_Owner reflect_statement_expression(const underworld::Expression &input_expression,
                                                               Scope &scope);
      overworld::Expression_Owner reflect_statement(const underworld::Expression &input_expression,
                                                    Scope &scope);
      overworld::Profession_Reference reflect_profession(const underworld::Profession *profession,
                                                         Scope &scope);
      overworld::Profession_Reference reflect_profession(const underworld::Profession &profession,
                                                         Scope &scope);

      overworld::Profession_Reference reflect_profession_child(overworld::Member &member,
                                                               const underworld::Profession &profession, Scope &scope);

      overworld::Profession_Reference reflect_dungeon_variant(const underworld::Dungeon_Variant &input_variant,
                                                              Scope &scope);

      overworld::Profession_Reference reflect_dungeon_reference(const underworld::Profession &profession,
                                                                Scope &scope);

      overworld::Profession_Reference reflect_dungeon_usage(const underworld::Token_Profession &profession,
                                                            Scope &scope);

      overworld::Minion &reflect_minion(const underworld::Minion &input_minion, Scope &output_scope);
      std::unique_ptr<overworld::Parameter>
      create_parameter(const underworld::Minion &input_minion, Scope &scope,
                       overworld::Function_Interface &function);
      std::unique_ptr<overworld::Minion> create_minion(const underworld::Minion &input_minion,
                                                       Scope &scope);

      overworld::Expression_Owner reflect_range(const underworld::Range &input_range, Scope &scope);

      overworld::Profession_Reference reflect_function_signature(const underworld::Function_Profession &input_signature,
                                                                 Scope &scope);

      void reflect_function_signature(const underworld::Function_Signature &input_signature,
                                      overworld::Function_Signature &function_profession,
                                      Scope &scope, Scope &function_scope);

      void reflect_function_with_block2(const underworld::Function_With_Block &input_function,
                                        overworld::Function_With_Block &output_function, Scope &scope);

//      void create_block_parameter(const underworld::Parameter &input_parameter,
//                                  overworld::Function_With_Block &output_function,
//                                  Scope &block_scope, Scope &scope);

      void reflect_function_with_block3(const underworld::Function_With_Block &input_function,
                                        overworld::Function_With_Block &output_function, Scope &scope);

      void reflect_function1(const underworld::Member &member, Scope &scope);
      void reflect_function2(const underworld::Function &input_function, Scope &scope);
      void reflect_function3(const underworld::Function &input_function, Scope &scope);
      void reflect_scope1(const underworld::Scope &input_scope, Scope &output_scope);
      void reflect_scope2(const underworld::Scope &input_scope, Scope &output_scope);
      void reflect_scope3(const underworld::Scope &input_scope, Scope &output_scope);
      void reflect_dungeon1(const underworld::Dungeon &input_dungeon, Scope &output_scope);
      overworld::Profession_Reference reflect_primitive(const underworld::Primitive &primitive);
      overworld::Expression_Owner reflect_invoke(const underworld::Invoke &function_call,
                                                 Scope &scope);
      overworld::Expression_Owner reflect_instantiation(const underworld::Instantiation &instantiation,
                                                        Scope &scope);

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
      Mirror(overworld::Profession_Library &profession_library, Element_Map &element_map, overworld::Graph *graph,
             Temporary_Interface_Manager &temporary_interface_manager, overworld::File_Library &header_files) :
        profession_library(profession_library), element_map(element_map), graph(graph),
        temporary_interface_manager(temporary_interface_manager), header_files(header_files),
        connector(graph, profession_library) {}

      void reflect_root(const underworld::Dungeon &input, Scope &scope);
  };
}