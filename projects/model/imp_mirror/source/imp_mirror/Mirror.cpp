
#include <overworld/expressions/Block_Expression.h>
#include <overworld/expressions/Flow_Control.h>
#include <overworld/expressions/Literal.h>
#include <overworld/expressions/Return.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/schema/Function.h>
#include <overworld/expressions/Member_Expression.h>
#include <overworld/expressions/Operator.h>
#include <underworld/expressions/Assignment.h>
#include <overworld/expressions/Assignment.h>
#include <overworld/expressions/Self.h>
#include <overworld/expressions/Chain.h>
#include <overworld/expressions/Instantiation.h>
#include <overworld/schema/Temporary_Minion.h>
#include <underworld/schema/Enchantment.h>
#include <overworld/schema/Enchantment_With_Arguments.h>
#include "Mirror.h"
#include "Code_Error.h"

namespace imp_mirror {

  void Mirror::reflect_enchantments(const underworld::Enchantment_Array &input_enchantments,
                                    overworld::Enchantment_Container &output_enchantments,
                                    overworld::Scope &scope) {
    for (auto &input_enchantment: input_enchantments) {
      auto enchantment = profession_library.get_enchantment_library().find_enchantment(input_enchantment->get_name());
      if (!enchantment)
        throw std::runtime_error("Could not find enchantment " + input_enchantment->get_name());

      if (input_enchantment->get_arguments().size() > 0) {
        auto parent = dynamic_cast<overworld::Enchantment_With_Parameters *>(enchantment);
        auto complex_enchantment = new overworld::Enchantment_With_Arguments(*parent);
        output_enchantments.add_enchantment(overworld::Enchantment_Owner(complex_enchantment));
        for (auto &argument : input_enchantment->get_arguments()) {
          complex_enchantment->add_argument(reflect_expression(*argument, scope));
        }
      }
      else {
        output_enchantments.add_enchantment(*enchantment);
      }
    }
  }

  void Mirror::apply_node_assignment(overworld::Node &target, overworld::Node &value) {
    graph.connect(target, value);

    // Optimization to reduce the amount of graph solving later on
    // since so often variables types are defined by assigning them an instantiation.
    auto &value_profession = value.get_element().get_profession();
    if (target.get_element().get_profession().get_type()
        == overworld::Profession_Type::unknown
        && value_profession.get_type()
           != overworld::Profession_Type::unknown) {
      target.get_element().set_profession(value_profession);
      target.set_resolved(true);
    }
  }

  overworld::Expression_Owner Mirror::reflect_assignment(const underworld::Assignment &input_assignment,
                                                         overworld::Scope &scope) {
    auto target = reflect_expression(*input_assignment.get_target(), scope);
    auto &last = target->get_last();
    auto operator_type = reflect_operator(input_assignment.get_operator());
    auto value = reflect_expression(*input_assignment.get_value(), scope);

    apply_node_assignment(*last.get_node(), *value->get_node());
    graph.connect(*last.get_node(), *value->get_node());
    return overworld::Expression_Owner(new overworld::Assignment(target, operator_type, value));
  }

  overworld::Expression_Owner Mirror::reflect_literal(const underworld::Literal &input_literal,
                                                      overworld::Dungeon &dungeon,
                                                      overworld::Function_Interface *function) {

    overworld::Literal *expression;
    switch (input_literal.get_primitive_type()) {

      case underworld::Primitive_Type::Bool:
        expression = new overworld::Literal_Bool(
          (*dynamic_cast<const underworld::Literal_Bool *>(&input_literal)).get_value(), &dungeon,
          input_literal.get_source_point(), function);
        break;

      case underworld::Primitive_Type::Int:
        expression = new overworld::Literal_Int(
          (*(const underworld::Literal_Int *) &input_literal).get_value(),
          &dungeon, input_literal.get_source_point(), function);
        break;

      case underworld::Primitive_Type::String:
        expression = new overworld::Literal_String(
          (*dynamic_cast<const underworld::Literal_String *>(&input_literal)).get_value(), &dungeon,
          input_literal.get_source_point(), function);
        break;

      default:
        throw std::runtime_error("Not implemented.");
    }

    return overworld::Expression_Owner(expression);
  }

  overworld::Expression_Owner Mirror::reflect_member(const underworld::Member_Expression &input_member_expression,
                                                     overworld::Scope &scope) {
    auto name = input_member_expression.get_name();
    auto input_member = scope.find_member(name);
    if (!input_member)
      throw Code_Error("Unknown symbol " + name, input_member_expression.get_source_point());

    return overworld::Expression_Owner(new overworld::Member_Expression(*input_member,
                                                                        input_member_expression.get_source_point()));
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
    graph.connect(scope.get_function()->get_node(), *expression->get_node());
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
    auto &input_minion = input_declaration.get_minion();
    auto variable = new overworld::Minion(input_minion.get_name(),
                                          reflect_profession(input_minion.get_profession(), scope),
                                          nullptr,
                                          input_minion.get_source_point(),
                                          scope.get_function());
    scope.add_minion(variable);
    auto expression = reflect_expression(input_declaration.get_expression(), scope);
    apply_node_assignment(variable->get_node(), *expression->get_node());
    return overworld::Expression_Owner(new overworld::Minion_Declaration_And_Assignment(*variable, expression));
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
    auto output_block = new overworld::Block_Expression(&scope);
    auto result = overworld::Expression_Owner(output_block);
    reflect_block(input_block, output_block->get_block());
    return result;
  }

  overworld::Function_Signature &Mirror::get_function_signature(overworld::Expression &expression,
                                                                std::vector<overworld::Expression_Owner> &arguments,
                                                                overworld::Scope &scope) {
    if (expression.get_type() == overworld::Expression::Type::member) {
      auto member_expression = dynamic_cast<const overworld::Member_Expression *>(&expression);
      auto &member = member_expression->get_member();
      if (member.get_member_type() == overworld::Member_Type::function) {
        return static_cast<overworld::Member_Function *>(&member)->get_function().get_signature();
      }
      else if (member.get_member_type() == overworld::Member_Type::dungeon) {
        auto &dungeon = static_cast<overworld::Member_Dungeon *>(&member)->get_dungeon();
        return dungeon.get_or_create_constructor().get_signature();
      }
      else if (member.get_member_type() == overworld::Member_Type::minion) {
        auto member_minion = static_cast<overworld::Member_Minion *>(&member);
        auto &temporary_member = dynamic_cast<overworld::Temporary_Minion &>(member_minion->get_minion());
        auto &signature = temporary_member.get_or_create_signature();
        for (auto i = signature.get_parameters().size(); i < arguments.size(); ++i) {
          auto &argument = arguments[i];
          auto &argument_profession = argument->get_node()->get_element().get_profession();
          auto parameter = new overworld::Parameter("(temp)", argument_profession,
                                                    source_mapping::Source_Range(), *scope.get_function());
          temporary_member.add_parameter(parameter);
        }
        return signature;
      }
      else {
        throw std::runtime_error("Expression is not a function.");
      }

    }

    throw std::runtime_error("Expression is not a function.");
  }

  overworld::Minion &find_member_container(overworld::Expression &expression) {
    auto &chain = dynamic_cast<overworld::Chain &>(expression);
    auto &member_expression = dynamic_cast<overworld::Member_Expression &>( chain.get_first());
    auto &member_minion = static_cast<overworld::Member_Minion &>(member_expression.get_member());
    auto &minion = member_minion.get_minion();
    return minion;
  }

  overworld::Expression_Owner Mirror::reflect_invoke(const underworld::Invoke &function_call,
                                                     overworld::Scope &scope) {
    auto &input_expression = function_call.get_expression();
    auto output_expression = reflect_expression(input_expression, scope);

    auto &source_arguments = function_call.get_arguments();
    std::vector<overworld::Expression_Owner> arguments;
    arguments.reserve(source_arguments.size());
    for (auto &source_argument : source_arguments) {
      arguments.push_back(reflect_expression(*source_argument, scope));
    }

    auto &overworld_function = get_function_signature(output_expression->get_last(), arguments, scope);

    auto invoke = new overworld::Invoke(output_expression, arguments,
                                        function_call);
    auto result = overworld::Expression_Owner(invoke);

    auto &parameters = overworld_function.get_parameters();
    auto &invoke_arguments = invoke->get_arguments();
    if (parameters.size() != invoke_arguments.size())
      throw std::runtime_error("Parameter and argument lengths do not match.");

    for (int i = 0; i < invoke_arguments.size(); ++i) {
      auto &first = parameters[i]->get_node();
      auto &second = *invoke_arguments[i]->get_node();
      auto k = first.get_profession_reference().get_element_type();
      if (first.get_profession_reference().get_element_type() == overworld::Element_Type::parameter
          && first.get_function() != second.get_function()
          && first.get_profession().get_type() == overworld::Profession_Type::generic_parameter) {
        auto &member_container = find_member_container(invoke->get_expression());
        auto argument_node = new overworld::Argument_Node(first.get_profession(), member_container,
                                                          scope.get_function(), profession_library,
                                                          function_call.get_arguments()[i]->get_source_point());
        invoke->add_argument_node(std::unique_ptr<overworld::Argument_Node>(argument_node));
        graph.connect(*argument_node, second);
      }
      else {
        graph.connect(first, second);
      }
    }

    return result;
  }

  overworld::Expression_Owner Mirror::reflect_instantiation(const underworld::Instantiation &instantiation,
                                                            overworld::Scope &scope) {
    auto &input_profession_expression = instantiation.get_profession_expression();
    auto &output_profession = reflect_expression(input_profession_expression,
                                                 scope)->get_last().get_node()->get_profession();
//      auto &output_profession = reflect_profession(input_profession, scope);
    if (output_profession.get_type() == overworld::Profession_Type::unknown)
      throw Code_Error("Could not instantiate type " + output_profession.get_name(), instantiation.get_source_point());

    auto &source_arguments = instantiation.get_dictionary();
    auto function = scope.get_function();
    auto output_instantiation = new overworld::Instantiation(output_profession, instantiation.get_source_point(),
                                                             function ? nullptr : &scope.get_dungeon(), function);
    overworld::Expression_Owner result(output_instantiation);
    graph.connect(*output_instantiation->get_node(), output_profession.get_node());

    for (auto &pair : source_arguments) {
      auto name = pair.first;
      auto input_member = scope.find_member(name);
      if (!input_member)
        throw Code_Error("Unknown symbol " + name, instantiation.get_source_point());

      auto &minion = cast<overworld::Minion>(*input_member);
      auto value = reflect_expression(*pair.second, scope);
      graph.connect(minion.get_node(), *value->get_node());
      output_instantiation->add_expression(minion, std::move(value));
    }

    return result;
  }

  overworld::Expression_Owner create_temporary_member(overworld::Member_Expression &previous_expression,
                                                      const underworld::Member_Expression &member_expression,
                                                      overworld::Scope &scope) {
    auto &member = previous_expression.get_member();
    if (member.get_member_type() == overworld::Member_Type::minion) {
      auto &member_minion = static_cast<overworld::Member_Minion &>(member);
      auto &minion = *dynamic_cast<overworld::Parameter *>(&member_minion.get_minion());
      auto &interface = minion.get_or_create_interface();
      auto new_member = new overworld::Temporary_Minion(member_expression.get_name(),
                                                        overworld::Profession_Library::get_unknown(),
                                                        member_expression.get_source_point(), *scope.get_function());
      auto &member = interface.add_minion(new_member);
      auto result = new overworld::Member_Expression(member);
      new_member->add_expression(*result);
      return overworld::Expression_Owner(result);
    }
//          else if (member.get_member_type() == overworld::Member_Type::function) {
//            auto &minion = cast<overworld::Function>(member);
//            auto &interface = temporary_interface_manager.get_or_create_interface(minion);
//            auto &function = interface.create_function(member_expression.get_name(), profession_library.get_unknown());
//            return overworld::Expression_Owner(new overworld::Member_Expression(function));
//          }
    throw std::runtime_error("Not implemented.");
  }

  overworld::Expression_Owner Mirror::reflect_chain_member(overworld::Expression &first,
                                                           const underworld::Expression &second,
                                                           overworld::Scope &scope) {

    auto &previous_expression = first.get_last();
    auto &profession = previous_expression.get_node()->get_profession_reference().get_profession();
    if (second.get_type() == underworld::Expression::Type::member) {
      auto &member_expression = cast<underworld::Member_Expression>(second);
      if (profession.get_type() == overworld::Profession_Type::dungeon
          || profession.get_type() == overworld::Profession_Type::variant) {
        auto &dungeon = get_dungeon_interface(first.get_last())->get_original();
        auto member = dungeon.get_member_or_null(member_expression.get_name());
        if (!member)
          throw std::runtime_error("Could not find member.");

        return overworld::Expression_Owner(new overworld::Member_Expression(*member));
      }
      else if (profession.get_type() == overworld::Profession_Type::unknown) {
        if (previous_expression.get_type() == overworld::Expression::Type::member) {
          auto &previous_member_expression = *dynamic_cast<overworld::Member_Expression *>(&previous_expression);
          return create_temporary_member(previous_member_expression, member_expression, scope);
        }
      }

      throw std::runtime_error("Not implemented.");
    }
    else if (second.get_type() == underworld::Expression::Type::chain) {

      auto &chain = cast<underworld::Chain>(second);
      return reflect_chain(chain, scope);
    }

    throw std::runtime_error("Not implemented.");
  }

  overworld::Expression_Owner Mirror::reflect_chain(const underworld::Chain &input_chain, overworld::Scope &scope) {
    auto first = reflect_expression(input_chain.get_first(), scope);
    auto second = reflect_chain_member(*first, input_chain.get_second(), scope);
    auto chain = new overworld::Chain(first, second,
                                      scope.get_dungeon_if_not_function(),
                                      scope.get_function());
    overworld::Expression_Owner result(chain);
//    graph.connect(*first->get_node(), *second->get_node()).set_type(overworld::Connection_Type::member);
    return result;
  }

  overworld::Expression_Owner Mirror::reflect_expression(const underworld::Expression &input_expression,
                                                         overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression::Type::literal:
        return reflect_literal(*dynamic_cast<const underworld::Literal *>(&input_expression),
                               scope.get_dungeon(), scope.get_function());

      case underworld::Expression::Type::member:
        return reflect_member(
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

      case underworld::Expression::Type::invoke:
        return reflect_invoke(*dynamic_cast<const underworld::Invoke *>(&input_expression),
                              scope);

      case underworld::Expression::Type::instantiation:
        return reflect_instantiation(*dynamic_cast<const underworld::Instantiation *>(&input_expression),
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

  overworld::Profession &Mirror::reflect_primitive(const underworld::Primitive &primitive) {
    const auto index = (int) primitive.get_primitive_type();
    auto primitive_type = (overworld::Primitive_Type) index;
    return profession_library.get_primitive(primitive_type);
  }

  overworld::Profession &Mirror::reflect_profession_child(overworld::Member &member,
                                                          const underworld::Profession &profession) {
    if (member.get_member_type() == overworld::Member_Type::dungeon) {
      auto &dungeon = get_dungeon(member);
      auto child = dungeon.get_member_or_null(profession.get_name());
      if (!child)
        throw Code_Error("Could not find " + profession.get_name(), profession.get_source_point());

      if (profession.get_type() == underworld::Profession_Type::dungeon_reference) {
        return reflect_dungeon_reference(profession, dungeon);
      }
      else if (profession.get_type() == underworld::Profession_Type::token) {

        return get_possible_generic_dungeon(get_dungeon(*child));
      }
      throw std::runtime_error("Not implemented");
    }

    throw std::runtime_error("Not implemented");
  }

  overworld::Profession &Mirror::reflect_dungeon_reference(const underworld::Profession &profession,
                                                           overworld::Scope &scope) {
    auto input_dungeon = dynamic_cast<const underworld::Dungeon_Reference_Profession *>(&profession);
    auto member = scope.find_member(input_dungeon->get_name());
    if (!member)
      throw Code_Error("Could not find " + input_dungeon->get_name(), input_dungeon->get_source_point());

    return reflect_profession_child(*member, input_dungeon->get_child());
  }

  overworld::Dungeon_Interface &Mirror::get_possible_generic_dungeon(overworld::Dungeon &dungeon) {
    auto &generic_parameters = dungeon.get_generic_parameters();
    if (generic_parameters.size() > 0) {
      std::vector<overworld::Profession *> professions;
      professions.reserve(generic_parameters.size());
      for (auto parameter : generic_parameters) {
        professions.push_back(&profession_library.get_unknown());
      }
      return profession_library.get_or_create_dungeon_variant(dungeon, professions, graph);
    }

    return dungeon;
  }

  overworld::Profession &Mirror::reflect_dungeon_usage(const underworld::Token_Profession &token,
                                                       overworld::Scope &scope) {
    auto member = scope.find_member(token.get_name());
    if (!member)
      throw Code_Error("Could not find " + token.get_name(), token.get_source_point());

    if (member->get_member_type() == overworld::Member_Type::dungeon) {
      auto member_dungeon = static_cast<overworld::Member_Dungeon *>(member);
      auto &dungeon = member_dungeon->get_dungeon();
      return get_possible_generic_dungeon(dungeon);
    }
    else if (member->get_member_type() == overworld::Member_Type::profession) {
      auto member_dungeon = static_cast<overworld::Member_Profession *>(member);
      return member_dungeon->get_profession();
    }
    else {
      throw new Code_Error(token.get_name() + " is not a dungeon.", token.get_source_point());
    }
  }

  overworld::Profession &Mirror::reflect_profession(const underworld::Profession &profession, overworld::Scope &scope) {
    switch (profession.get_type()) {
      case underworld::Profession_Type::primitive:
        return reflect_primitive(cast<underworld::Primitive>(profession));

      case underworld::Profession_Type::dungeon_reference: {
        return reflect_dungeon_reference(profession, scope);
      }

      case underworld::Profession_Type::token: {
        auto &token = cast<underworld::Token_Profession>(profession);
        return reflect_dungeon_usage(token, scope);
      }

      case underworld::Profession_Type::unknown:
        return profession_library.get_unknown();

      case underworld::Profession_Type::reference: {
        auto &child_profession = cast<underworld::Reference>(profession).get_profession();
        return profession_library.get_reference(reflect_profession(child_profession, scope));
      }

      case underworld::Profession_Type::pointer: {
        auto &child_profession = static_cast<const underworld::Pointer &>(profession).get_profession();
        return profession_library.get_pointer(reflect_profession(child_profession, scope));
      }
    }

    throw std::runtime_error("Not implemented");
  }

  overworld::Profession &Mirror::reflect_profession(const underworld::Profession *profession, overworld::Scope &scope) {
    if (!profession)
      return profession_library.get_unknown();

    return reflect_profession(*profession, scope);
  }

  void Mirror::reflect_function1(const underworld::Member &member, overworld::Scope &scope) {
    auto &input_function = *(dynamic_cast<const underworld::Function *>(&member));
    auto input_function_with_block = (dynamic_cast<const underworld::Function_With_Block *>(&member));
    if (input_function_with_block) {
      auto output_function = new overworld::Function_With_Block(member.get_name(),
                                                                scope, scope.get_dungeon(),
                                                                member.get_source_point());
      scope.add_function(std::unique_ptr<overworld::Function>(output_function));

      reflect_scope1(input_function_with_block->get_block().get_scope(),
                     output_function->get_block().get_scope());
//      if (input_function.is_static())
//        output_function->add_enchantment(();
      reflect_enchantments(input_function.get_enchantments(), output_function->get_enchantments(), scope);

      element_map.add(&input_function, output_function);
    }
    else {
      auto output_function = new overworld::Virtual_Function(member.get_name(),
                                                             scope, scope.get_dungeon(), member.get_source_point());
      scope.add_function(std::unique_ptr<overworld::Function>(output_function));
//      output_function->set_is_static(input_function.is_static());
      element_map.add(&input_function, output_function);
    }

  }

  void Mirror::reflect_function2(const underworld::Function &input_function) {
    auto input_function_with_block = dynamic_cast<const underworld::Function_With_Block *>(&input_function);
    if (input_function_with_block) {
      auto output_function_with_block = element_map.find_or_null<overworld::Function_With_Block>(&input_function);
      auto &profession = reflect_profession(input_function.get_profession(),
                                            *output_function_with_block->get_scope().get_parent());
      reflect_scope2(
        input_function_with_block->get_block().get_scope(),
        output_function_with_block->get_block().get_scope());

      auto &parameters = input_function_with_block->get_parameters();
      for (auto &source_parameter : parameters) {
        auto &minion = output_function_with_block->get_block().get_scope().get_minion(source_parameter->get_name());
        output_function_with_block->add_parameter_to_signature(cast<overworld::Parameter>(minion));
      }
    }
    else {
      auto virtual_input_function = dynamic_cast<const underworld::Virtual_Function *>(&input_function);
      for (auto &source_parameter : virtual_input_function->get_parameters()) {
        auto output_function = element_map.find_or_null<overworld::Virtual_Function>(&input_function);
        auto minion = create_parameter(*source_parameter, output_function->get_parent_scope(), *output_function);
        output_function->add_parameter(std::move(minion));
      }
    }
  }

  void Mirror::reflect_function3(const underworld::Function &input_function) {
    auto input_function_with_block = (dynamic_cast<const underworld::Function_With_Block *>(&input_function));
    if (input_function_with_block) {
      auto &output_function = *element_map.find_or_null<overworld::Function_With_Block>(&input_function);
      reflect_block(input_function_with_block->get_block(), output_function.get_block());
      output_function.finalize(profession_library);
    }
  }

  std::unique_ptr<overworld::Parameter> Mirror::create_parameter(const underworld::Minion &input_minion,
                                                                 overworld::Scope &scope,
                                                                 overworld::Function_Interface &function) {
    auto profession = &reflect_profession(input_minion.get_profession(), scope);
    return std::unique_ptr<overworld::Parameter>(
      new overworld::Parameter(input_minion.get_name(), *profession,
                               input_minion.get_source_point(), function)
    );
  }

  std::unique_ptr<overworld::Minion> Mirror::create_minion(const underworld::Minion &input_minion,
                                                           overworld::Scope &scope) {
    if (input_minion.is_parameter()) {
      return create_parameter(input_minion, scope, *scope.get_function());
    }

    auto profession = &reflect_profession(input_minion.get_profession(), scope);
    if (profession->get_type() == overworld::Profession_Type::reference)
      profession = &profession_library.get_pointer(profession->get_base());

    auto function = scope.get_function();
    return std::unique_ptr<overworld::Minion>(
      new overworld::Minion(input_minion.get_name(), *profession, function ? nullptr : &scope.get_dungeon(),
                            input_minion.get_source_point(), scope.get_function())
    );
  }

  void Mirror::reflect_minion(const underworld::Minion &input_minion, overworld::Scope &output_scope) {
    auto output_minion = create_minion(input_minion, output_scope);
    element_map.add(&input_minion, output_minion.get());
    reflect_enchantments(input_minion.get_enchantments(), output_minion->get_enchantments(), output_scope);
    output_scope.add_minion(std::move(output_minion));
  }

  void Mirror::reflect_dungeon1(const underworld::Dungeon &input_dungeon, overworld::Scope &output_scope) {
    overworld::Dungeon *output_dungeon;
    auto existing_member = output_scope.get_member_or_null(input_dungeon.get_name());
    if (existing_member) {
      auto member_dungeon = static_cast<overworld::Member_Dungeon *>(existing_member);
      output_dungeon = &member_dungeon->get_dungeon();
      auto dungeon = element_map.find_or_null<overworld::Dungeon>(&input_dungeon);
      if (!dungeon)
        element_map.add(&input_dungeon, output_dungeon);
    }
    else {
      output_dungeon = new overworld::Dungeon(input_dungeon.get_name(), output_scope,
                                              input_dungeon.get_source_point());
      auto output_dungeon_owner = std::unique_ptr<overworld::Dungeon>(output_dungeon);

      element_map.add(&input_dungeon, output_dungeon);
      output_scope.add_dungeon(std::move(output_dungeon_owner));
    }

    for (auto &generic_parameter: input_dungeon.get_generic_parameters()) {
      auto output_generic_parameter = new overworld::Generic_Parameter(generic_parameter, output_dungeon, nullptr);
      output_dungeon->add_generic_parameter(std::unique_ptr<overworld::Generic_Parameter>(output_generic_parameter));
    }
    reflect_enchantments(input_dungeon.get_enchantments(), output_dungeon->get_enchantments(), output_scope);

    auto &enchantment_library = profession_library.get_enchantment_library();
    if (output_dungeon->has_enchantment(enchantment_library.get_value())) {
      output_dungeon->set_default_ownership(overworld::Ownership::value);
    }

    auto external = output_dungeon->get_enchantments().get_enchantment(enchantment_library.get_external());

    if (external) {
      auto value = external->get_argument_string(0);
      if (value != "") {
        output_dungeon->set_file(std::unique_ptr<overworld::File>(new overworld::External_File(value)));
      }
    }

    reflect_scope1(input_dungeon, *output_dungeon);
  }

  void Mirror::reflect_scope1(const underworld::Scope &input_scope, overworld::Scope &output_scope) {

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        reflect_function1(*input_member.second, output_scope);
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::profession) {
        auto input_profession = cast<const underworld::Profession_Member>(*input_member.second)
          .get_profession();

        if (input_profession->get_type() == underworld::Profession_Type::dungeon) {
          auto &input_dungeon = cast<underworld::Dungeon>(*input_profession);
          reflect_dungeon1(input_dungeon, output_scope);
        }
      }
    }
  }

  void Mirror::reflect_scope2(const underworld::Scope &input_scope, overworld::Scope &output_scope) {

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::minion) {
        auto &input_variable = *(dynamic_cast<const underworld::Minion *>(input_member.second.get()));
        reflect_minion(input_variable, output_scope);
      }
    }

    if (output_scope.get_scope_type() == overworld::Scope_Type::dungeon) {
      auto &input_dungeon = cast<underworld::Dungeon>(input_scope);
      auto &output_dungeon = cast<overworld::Dungeon>(output_scope);
      for (auto &contract:input_dungeon.get_contracts()) {
        auto &dungeon = cast<overworld::Dungeon>(reflect_profession(*contract, *output_dungeon.get_parent()));
        output_dungeon.add_contract(dungeon);
      }
    }
    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        reflect_function2(cast<underworld::Function>(*input_member.second));
      }
      else if (input_member.second->get_type() == underworld::Member::Type::minion) {
        auto &input_variable = *(dynamic_cast<const underworld::Minion *>(input_member.second.get()));
        auto &profession = reflect_profession(input_variable.get_profession(), output_scope);
        auto &output_minion = *element_map.find_or_null<overworld::Minion>(&input_variable);
        profession_library.assign(output_minion.get_node(), profession);
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::profession) {
        auto input_profession = cast<const underworld::Profession_Member>(*input_member.second)
          .get_profession();

        if (input_profession->get_type() == underworld::Profession_Type::dungeon) {
          auto &input_dungeon = cast<underworld::Dungeon>(*input_profession);
          auto dungeon = element_map.find_or_null<overworld::Dungeon>(&input_dungeon);
          if (!dungeon)
            throw Code_Error("Could not find overworld dungeon " + input_profession->get_name(),
                             input_dungeon.get_source_point());

          reflect_scope2(input_dungeon, *dungeon);
        }
      }
    }
  }

  void Mirror::reflect_scope3(const underworld::Scope &input_scope, overworld::Scope &output_scope) {
    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        reflect_function3(cast<underworld::Function_With_Block>(*input_member.second));
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::profession) {
        auto input_profession = cast<const underworld::Profession_Member>(*input_member.second)
          .get_profession();

        if (input_profession->get_type() == underworld::Profession_Type::dungeon) {
          auto &input_dungeon = cast<underworld::Dungeon>(*input_profession);
          auto &dungeon = *element_map.find_or_null<overworld::Dungeon>(&input_dungeon);
          reflect_scope3(input_dungeon, dungeon);
        }
      }
    }
  }

  void Mirror::reflect_root(const underworld::Dungeon &input, overworld::Dungeon &output) {
    reflect_scope1(input, output);
    reflect_scope2(input, output);
    reflect_scope3(input, output);
  }
}