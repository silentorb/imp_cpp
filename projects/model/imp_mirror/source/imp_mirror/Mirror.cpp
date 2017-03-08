
#include <overworld/expressions/Block.h>
#include <overworld/expressions/Flow_Control.h>
#include <overworld/expressions/Literal.h>
#include <overworld/expressions/Return.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/schema/Function.h>
#include "Mirror.h"

namespace imp_mirror {

  overworld::Expression_Owner Mirror::reflect_literal(const underworld::Literal &input_literal) {

    switch (input_literal.get_primitive_type()) {

      case underworld::Primitive_Type::Bool:
        return overworld::Expression_Owner(new overworld::Literal_Bool(
          (*dynamic_cast<const underworld::Literal_Bool *>(&input_literal)).get_value())
        );

      case underworld::Primitive_Type::Int:
        return overworld::Expression_Owner(new overworld::Literal_Int(
          (*dynamic_cast<const underworld::Literal_Int *>(&input_literal)).get_value())
        );

      case underworld::Primitive_Type::String:
        return overworld::Expression_Owner(new overworld::Literal_String(
          (*dynamic_cast<const underworld::Literal_String *>(&input_literal)).get_value())
        );

      default:
        throw std::runtime_error("Not implemented.");
    }
  }

  overworld::Expression_Owner Mirror::reflect_return_nothing(const underworld::Return &input_return) {
    return overworld::Expression_Owner(new overworld::Return());
  }

  overworld::Expression_Owner Mirror::reflect_return_with_value(const underworld::Return_With_Value &input_return) {
    return overworld::Expression_Owner(new overworld::Return_With_Value(reflect_expression(input_return.get_value())));
  }

  overworld::Expression_Owner
  Mirror::reflect_variable_declaration(const underworld::Minion_Declaration &input_declaration,
                                       overworld::Scope &scope) {
    auto &variable = scope.get_minion(input_declaration.get_minion().get_name());
    return overworld::Expression_Owner(new overworld::Minion_Declaration(variable));
  }

  overworld::Expression_Owner Mirror::reflect_variable_declaration_with_assignment(
    const underworld::Minion_Declaration_And_Assignment &input_declaration, overworld::Scope &scope) {
    auto &variable = scope.get_minion(input_declaration.get_minion().get_name());
    auto expression = reflect_expression(input_declaration.get_expression());
    return overworld::Expression_Owner(new overworld::Minion_Declaration_And_Assignment(variable, expression));
  }

  overworld::Expression_Owner Mirror::reflect_if(const underworld::If &input_if,
                                                 overworld::Scope &scope) {
    return overworld::Expression_Owner(new overworld::If(
      reflect_expression(input_if.get_condition()),
      reflect_statement(input_if.get_expression(), scope)
    ));
  }

  void Mirror::reflect_block(const underworld::Block &input_block, overworld::Block &output_block) {
    for (auto &input_expression : input_block.get_expressions()) {
      auto output_expression = reflect_statement(*input_expression, output_block.get_scope());
      output_block.add_expression(output_expression);
    }
  }

  overworld::Expression_Owner Mirror::reflect_block(const underworld::Block &input_block) {
    auto output_block = new overworld::Block(input_block);
    auto result = overworld::Expression_Owner(output_block);
    reflect_block(input_block, *output_block);
    return result;
  }

  overworld::Expression_Owner Mirror::reflect_expression(const underworld::Expression &input_expression) {
    switch (input_expression.get_type()) {

      case underworld::Expression::Type::literal:
        return reflect_literal(*dynamic_cast<const underworld::Literal *>(&input_expression));

      default:
        throw std::runtime_error(" Not implemented.");
    }
  }

  overworld::Expression_Owner Mirror::reflect_statement(const underworld::Expression &input_expression,
                                                        overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression::Type::block:
        return reflect_block(*dynamic_cast<const underworld::Block *>(&input_expression));

      case underworld::Expression::Type::If:
        return reflect_if(*dynamic_cast<const underworld::If *>(&input_expression), scope);

      case underworld::Expression::Type::return_nothing:
        return reflect_return_nothing(*dynamic_cast<const underworld::Return *>(&input_expression));

      case underworld::Expression::Type::return_with_value:
        return reflect_return_with_value(*dynamic_cast<const underworld::Return_With_Value *>(&input_expression));

      case underworld::Expression::Type::variable_declaration:
        return reflect_variable_declaration(
          *dynamic_cast<const underworld::Minion_Declaration *>(&input_expression), scope);

      case underworld::Expression::Type::variable_declaration_and_assignment:
        return reflect_variable_declaration_with_assignment(
          *dynamic_cast<const underworld::Minion_Declaration_And_Assignment *>(&input_expression), scope);

      default:
        throw std::runtime_error(" Not implemented.");
    }
  }

  const overworld::Profession &Mirror::reflect_profession(const underworld::Profession &profession) {
    return profession_library.get_unknown();
  }

  void Mirror::reflect_dungeon(const underworld::Dungeon &input, overworld::Dungeon &output) {
    for (auto &entry: input.get_members()) {
      auto &member = *entry.second;
      if (member.get_type() == underworld::Member::Type::variable) {
        auto &input_minion = *dynamic_cast<underworld::Minion *>(&member);
        auto &profession = reflect_profession(input_minion.get_profession());
        auto &output_minion = output.create_minion(input_minion, profession);
      }
      else {
        auto &input_function = *dynamic_cast<underworld::Function *>(&member);
        auto &output_function = output.create_function(input_function);
        reflect_block(input_function.get_block(), output_function.get_block());
      }
    }

    for (auto &entry: input.get_dungeons()) {
      auto &input_dungeon = *entry.second;
      auto &output_dungeon = output.create_dungeon(input_dungeon);
      reflect_dungeon(input_dungeon, output_dungeon);
    }
  }
}