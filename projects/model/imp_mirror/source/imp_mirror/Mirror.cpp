
#include <overworld/expressions/Block_Expression.h>
#include <overworld/expressions/Flow_Control.h>
#include <overworld/expressions/Literal.h>
#include <overworld/expressions/Return.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/schema/Function.h>
#include <overworld/expressions/Minion_Expression.h>
#include <overworld/expressions/Operator.h>
#include <underworld/expressions/Assignment.h>
#include <overworld/expressions/Assignment.h>
#include <overworld/expressions/Self.h>
#include <overworld/expressions/Chain.h>
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

  overworld::Expression_Owner Mirror::reflect_block_expression(const underworld::Block &input_block,
                                                               overworld::Scope &scope) {
    auto output_block = new overworld::Block_Expression(input_block.get_scope(), &scope);
    auto result = overworld::Expression_Owner(output_block);
    reflect_block(input_block, output_block->get_block());
    return result;
  }

  overworld::Function &Mirror::get_function(overworld::Expression &expression) {
    throw std::runtime_error("Expression is not a function.");
//    if (expression.get_type() == overworld::Expression::Type::member) {
//      auto &member_expression = *dynamic_cast<const underworld::Member_Expression *>(&expression);
//      auto &member = member_expression.get_member();
//      if (member.get_type() == underworld::Member::Type::function) {
//        auto &underworld_function = *dynamic_cast<const underworld::Function *>(&member);
//        auto overworld_function = element_map.find_or_null<overworld::Function>(&underworld_function);
//        if (!overworld_function)
//          throw std::runtime_error("Could not find overworld function.");
//
//        return *overworld_function;
//      }
//      else if (member.get_type() == underworld::Member::Type::profession) {
//        auto &profession = member.get_profession();
//        auto &underworld_dungeon = *dynamic_cast<const underworld::Dungeon *>(&profession);
//        auto dungeon = element_map.find_or_null<overworld::Dungeon>(&underworld_dungeon);
//        if (!dungeon)
//          throw std::runtime_error("Could not find overworld dungeon.");
//
//        return dungeon->get_constructor();
////        if (!constructor)
////          throw std::runtime_error("Could not find constructor.");
////
////        return *constructor;
//      }
//      if (expression.get_type() == underworld::Expression::Type::unresolved_member) {
//        auto &member_expression = *dynamic_cast<const underworld::Unresolved_Member_Expression *>(&expression);
//        auto name = member_expression.get_member_name();
//
//
//      }
//      else {
//        throw std::runtime_error("Member is not a function.");
//      }
//
//    }
//    else if (expression.get_type() == underworld::Expression::Type::member) {
//      throw std::runtime_error("Not implemented.");
//    }
//    else {
//      throw std::runtime_error("Expression is not a function.");
//    }
  }

  overworld::Expression_Owner Mirror::reflect_function_call(const underworld::Function_Call &function_call,
                                                            overworld::Scope &scope) {
    auto &input_expression = function_call.get_expression();
    auto output_expression = reflect_expression(input_expression, scope);
    auto &overworld_function = get_function(output_expression->get_last());
//    auto overworld_function = element_map.find_or_null<overworld::Function>(&function_call.get_function());
//    if (!overworld_function)
//      throw std::runtime_error("Could not find function.");

    auto &source_arguments = function_call.get_arguments();
    std::vector<overworld::Expression_Owner> arguments;
    arguments.reserve(source_arguments.size());
    for (auto &source_argument : source_arguments) {
      arguments.push_back(reflect_expression(*source_argument, scope));
    }

    // Note: need to convert direct function references here to more open-ended expressions like I just did
    // with underworld.
    auto &parameters = overworld_function.get_parameters();
    for (int i = 0; i < arguments.size(); ++i) {
      graph.connect(parameters[i]->get_node(), *arguments[i]->get_node());
    }

    return overworld::Expression_Owner(new overworld::Function_Call(overworld_function, arguments, function_call));
  }

  overworld::Expression_Owner Mirror::reflect_chain(const underworld::Chain &input_chain, overworld::Scope &scope) {
    auto first = reflect_expression(input_chain.get_first(), scope);
    auto second = reflect_expression(input_chain.get_second(), scope);
    return overworld::Expression_Owner(new overworld::Chain(first, second));
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

      case underworld::Expression::Type::self:
        return overworld::Expression_Owner(new overworld::Self(scope.get_dungeon()));

      case underworld::Expression::Type::chain:
        return reflect_chain(*dynamic_cast<const underworld::Chain *>(&input_expression), scope);

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
        return reflect_block_expression(cast<const underworld::Block>(input_expression), scope);

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

  void Mirror::reflect_function1(const underworld::Member &member, overworld::Scope &scope) {
    auto &input_function = *(dynamic_cast<const underworld::Function *>(&member));
    auto &profession = reflect_profession(input_function.get_profession());
    auto &output_function = scope.create_function(input_function, profession, graph);

    reflect_scope1(input_function.get_block().get_scope(), output_function.get_block().get_scope());

    element_map.add(&input_function, &output_function);
  }

  void Mirror::reflect_function2(const underworld::Function &input_function) {
    auto &output_function = *element_map.find_or_null<overworld::Function>(&input_function);
    reflect_scope2(input_function.get_block().get_scope(), output_function.get_block().get_scope());

    for (auto &source_parameter : input_function.get_parameters()) {
      auto &minion = output_function.get_block().get_scope().get_minion(source_parameter->get_name());
      output_function.add_parameter(minion);
    }
    reflect_block(input_function.get_block(), output_function.get_block());
    output_function.finalize(profession_library);
  }

  void Mirror::reflect_scope1(const underworld::Scope &input_scope, overworld::Scope &output_scope) {

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
        reflect_function1(*input_member.second, output_scope);
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::profession) {
        auto &input_profession = cast<const underworld::Profession_Member>(*input_member.second)
          .get_profession();

        if (input_profession.get_type() == underworld::Profession::Type::dungeon) {
          auto &input_dungeon = cast<underworld::Dungeon>(input_profession);
          auto output_dungeon = std::unique_ptr<overworld::Dungeon>(
            new overworld::Derived_Dungeon(input_dungeon, output_scope));

          auto &dungeon = *output_dungeon;
          element_map.add(&input_dungeon, &dungeon);
          output_scope.add_dungeon(output_dungeon);
          reflect_scope1(input_dungeon, dungeon);
        }
        else {
//          auto &profession = reflect_profession(input_profession);
//          auto &output_minion = output_scope.add_profession(profession);
//          element_map.add(&input_profession, &output_minion);
        }
      }
    }
  }

  void Mirror::reflect_scope2(const underworld::Scope &input_scope, overworld::Scope &output_scope) {
    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        reflect_function2(cast<underworld::Function>(*input_member.second));
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::profession) {
        auto &input_profession = cast<const underworld::Profession_Member>(*input_member.second)
          .get_profession();

        if (input_profession.get_type() == underworld::Profession::Type::dungeon) {
          auto &input_dungeon = cast<underworld::Dungeon>(input_profession);
          auto &dungeon = *element_map.find_or_null<overworld::Dungeon>(&input_dungeon);
          reflect_scope2(input_dungeon, dungeon);
        }
        else {
//          auto &profession = reflect_profession(input_profession);
//          auto &output_minion = output_scope.add_profession(profession);
//          element_map.add(&input_profession, &output_minion);
        }
      }
    }
  }

  void Mirror::reflect_dungeon(const underworld::Dungeon &input, overworld::Dungeon &output) {
    reflect_scope1(input, output);
    reflect_scope2(input, output);

//    for (auto &entry: input.get_dungeons()) {
//      auto &input_dungeon = *entry.second;
//      auto &output_dungeon = output.create_dungeon(input_dungeon);
//      reflect_dungeon(input_dungeon, output_dungeon);
//    }

//    integrity.verify_no_unknowns();
  }
}