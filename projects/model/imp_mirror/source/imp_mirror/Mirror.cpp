
#include <overworld/expressions/Block.h>
#include <overworld/expressions/Flow_Control.h>
#include <overworld/expressions/Literal.h>
#include <overworld/expressions/Return.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/schema/Function.h>
#include <overworld/expressions/Minion_Expression.h>
#include <overworld/expressions/Operator.h>
#include <underworld/expressions/Assignment.h>
#include <overworld/expressions/Assignment.h>
#include "Mirror.h"

namespace imp_mirror {

  overworld::Expression_Owner
  Mirror::reflect_assignment(const underworld::Assignment &input_assignment, overworld::Scope &scope) {
    auto target = reflect_expression(*input_assignment.get_target(), scope);
    auto operator_type = reflect_operator(input_assignment.get_operator());
    auto value = reflect_expression(*input_assignment.get_value(), scope);

    graph.connect(*target->get_node(), *value->get_node());

    return overworld::Expression_Owner(new overworld::Assignment(target, operator_type, value));
  }

  overworld::Expression_Owner Mirror::reflect_literal(const underworld::Literal &input_literal) {

    overworld::Literal *expression;
    switch (input_literal.get_primitive_type()) {

      case underworld::Primitive_Type::Bool:
        expression = new overworld::Literal_Bool(
          (*dynamic_cast<const underworld::Literal_Bool *>(&input_literal)).get_value(), input_literal);
        break;

      case underworld::Primitive_Type::Int:
        expression = new overworld::Literal_Int(
          (*dynamic_cast<const underworld::Literal_Int *>(&input_literal)).get_value(), input_literal);
        break;

      case underworld::Primitive_Type::String:
        expression = new overworld::Literal_String(
          (*dynamic_cast<const underworld::Literal_String *>(&input_literal)).get_value(), input_literal);
        break;

      default:
        throw std::runtime_error("Not implemented.");
    }

    graph.add_node(*expression->get_node());
    return overworld::Expression_Owner(expression);
  }

  overworld::Expression_Owner
  Mirror::reflect_minion(const underworld::Member_Expression &input_minion_expression, overworld::Scope &scope) {
    auto &input_minion = input_minion_expression.get_member();
    auto output_minion = element_map.find_or_null<overworld::Minion>(&input_minion);
    if (!output_minion)
      throw std::runtime_error("Could not find minion.");

    return overworld::Expression_Owner(new overworld::Minion_Expression(*output_minion));
  }

  overworld::Operator_Type Mirror::reflect_operator(const underworld::Operator &input_operator) {
    return (overworld::Operator_Type) (int) input_operator.get_operator_type();
  }

  overworld::Expression_Owner Mirror::reflect_return_nothing(const underworld::Return &input_return) {
    return overworld::Expression_Owner(new overworld::Return());
  }

  overworld::Expression_Owner Mirror::reflect_return_with_value(const underworld::Return_With_Value &input_return,
                                                                overworld::Scope &scope) {
    auto expression = reflect_expression(input_return.get_value(), scope);
    graph.connect(scope.get_function().get_node(), *expression->get_node());
    return overworld::Expression_Owner(
      new overworld::Return_With_Value(std::move(expression)));
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
    auto expression = reflect_expression(input_declaration.get_expression(), scope);
    graph.connect(variable.get_node(), *expression->get_node());
    return overworld::Expression_Owner(new overworld::Minion_Declaration_And_Assignment(variable, expression));
  }

  overworld::Expression_Owner Mirror::reflect_if(const underworld::If &input_if,
                                                 overworld::Scope &scope) {
    return overworld::Expression_Owner(new overworld::If(
      reflect_expression(input_if.get_condition(), scope),
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
    auto output_block = new overworld::Basic_Block(input_block);
    auto result = overworld::Expression_Owner(output_block);
    reflect_block(input_block, *output_block);
    return result;
  }

  overworld::Expression_Owner Mirror::reflect_function_call(const underworld::Function_Call &function_call,
                                                            overworld::Scope &scope) {

    auto overworld_function = element_map.find_or_null<overworld::Function>(&function_call.get_function());
    if (!overworld_function)
      throw std::runtime_error("Could not find function.");

    auto &source_arguments = function_call.get_arguments();
    std::vector<overworld::Expression_Owner> arguments;
    arguments.reserve(source_arguments.size());
    for (auto &source_argument : source_arguments) {
      arguments.push_back(reflect_expression(*source_argument, scope));
    }

    auto &parameters = overworld_function->get_parameters();
    for (int i = 0; i < arguments.size(); ++i) {
      graph.connect(parameters[i]->get_node(), *arguments[i]->get_node());
    }

    return overworld::Expression_Owner(new overworld::Function_Call(*overworld_function, arguments, function_call));
  }

  overworld::Expression_Owner Mirror::reflect_expression(const underworld::Expression &input_expression,
                                                         overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression::Type::literal:
        return reflect_literal(*dynamic_cast<const underworld::Literal *>(&input_expression));

      case underworld::Expression::Type::member:
        return reflect_minion(
          *dynamic_cast<const underworld::Member_Expression *>(&input_expression), scope);

      case underworld::Expression::Type::Operator:
        return overworld::Expression_Owner(new overworld::Operator(
          reflect_operator(*dynamic_cast<const underworld::Operator *>(&input_expression))
        ));

      default:
        return reflect_statement_expression(input_expression, scope);
    }
  }

  overworld::Expression_Owner Mirror::reflect_statement_expression(const underworld::Expression &input_expression,
                                                                   overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression::Type::function_call:
        return reflect_function_call(*dynamic_cast<const underworld::Function_Call *>(&input_expression),
                                     scope);

      default:
        throw std::runtime_error(" Not implemented.");
    }
  }

  overworld::Expression_Owner Mirror::reflect_statement(const underworld::Expression &input_expression,
                                                        overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression::Type::assignment:
        return reflect_assignment(*dynamic_cast<const underworld::Assignment *>(&input_expression), scope);

      case underworld::Expression::Type::block:
        return reflect_block(*dynamic_cast<const underworld::Block *>(&input_expression));

      case underworld::Expression::Type::If:
        return reflect_if(*dynamic_cast<const underworld::If *>(&input_expression), scope);

      case underworld::Expression::Type::return_nothing:
        return reflect_return_nothing(*dynamic_cast<const underworld::Return *>(&input_expression));

      case underworld::Expression::Type::return_with_value:
        return reflect_return_with_value(*dynamic_cast<const underworld::Return_With_Value *>(&input_expression),
                                         scope);

      case underworld::Expression::Type::variable_declaration:
        return reflect_variable_declaration(
          *dynamic_cast<const underworld::Minion_Declaration *>(&input_expression), scope);

      case underworld::Expression::Type::variable_declaration_and_assignment:
        return reflect_variable_declaration_with_assignment(
          *dynamic_cast<const underworld::Minion_Declaration_And_Assignment *>(&input_expression), scope);

      default:
        return reflect_statement_expression(input_expression, scope);
    }
  }

  const overworld::Profession &Mirror::reflect_primitive(const underworld::Primitive &primitive) {
    const auto index = (int) primitive.get_primitive_type();
    auto primitive_type = (overworld::Primitive_Type) index;
    return profession_library.get_primitive(primitive_type);
  }

  const overworld::Profession &Mirror::reflect_profession(const underworld::Profession &profession) {
    switch (profession.get_type()) {
      case underworld::Profession::Type::primitive:
        return reflect_primitive(*dynamic_cast<const underworld::Primitive *>(&profession));
    }
    return profession_library.get_unknown();
  }

  void Mirror::reflect_scope(const underworld::Scope &input_scope, overworld::Scope &output_scope) {

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::minion) {
        auto &input_variable = *(dynamic_cast<const underworld::Minion *>(input_member.second.get()));
        auto &profession = reflect_profession(input_variable.get_profession());
        auto &output_minion = output_scope.create_minion(input_variable, profession, graph);
//        integrity.check_reference(output_minion);
        element_map.add(&input_variable, &output_minion);
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        auto &input_function = *(dynamic_cast<const underworld::Function *>(input_member.second.get()));
        auto &profession = reflect_profession(input_function.get_return_type());
        auto &output_function = output_scope.create_function(input_function, profession, graph);

        reflect_scope(input_function.get_block().get_scope(), output_function.get_block().get_scope());

        for (auto &source_parameter : input_function.get_parameters()) {
          auto &minion = output_function.get_block().get_scope().get_minion(source_parameter->get_name());
          output_function.add_parameter(minion);
        }
        reflect_block(input_function.get_block(), output_function.get_block());
        output_function.finalize(profession_library);
        element_map.add(&input_function, &output_function);
      }
    }
  }

  void Mirror::reflect_dungeon(const underworld::Dungeon &input, overworld::Dungeon &output) {
    reflect_scope(input, output);

//    for (auto &entry: input.get_dungeons()) {
//      auto &input_dungeon = *entry.second;
//      auto &output_dungeon = output.create_dungeon(input_dungeon);
//      reflect_dungeon(input_dungeon, output_dungeon);
//    }

//    integrity.verify_no_unknowns();
  }
}