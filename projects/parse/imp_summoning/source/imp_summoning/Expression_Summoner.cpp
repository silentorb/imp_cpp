#include <underworld/expressions/Literal.h>
#include <underworld/expressions/Flow_Control.h>
#include <underworld/expressions/Minion_Declaration.h>
#include <underworld/expressions/Return.h>
#include <underworld/expressions/Assignment.h>
#include <underworld/expressions/Member_Expression.h>
#include <underworld/expressions/Invoke.h>
#include <underworld/expressions/Chain.h>
#include <underworld/expressions/Self.h>
#include <underworld/expressions/Instantiation.h>
#include <underworld/expressions/Lambda.h>
#include "Expression_Summoner.h"
#include "runic/common/exceptions.h"

using namespace underworld;

namespace imp_summoning {

  Expression_Summoner::Expression_Summoner(runic::Stream &input, Parser_Lookup &lookup) :
    Base_Summoner(input, lookup) {}

  Expression_Owner Expression_Summoner::process_variable_declaration(Context &context) {
//    auto &minion = context.get_scope().create_minion(input.expect_next(lexicon.identifier).get_text(),
//                                                      input.get_source_point());
    auto name = input.expect_next(lexicon.identifier).get_text();
    Minion_Owner minion(new Minion(name, input.get_source_point()));
    if (input.next().is(lexicon.assignment)) {
      input.next();
      auto expression = process_expression(context);
      return Expression_Owner(new Minion_Declaration_And_Assignment(minion, expression));
    }
    else {
      return Expression_Owner(new Minion_Declaration(minion));
    }
  }

  Expression_Owner Expression_Summoner::process_lambda(Member_Expression &parameter_info, Context &context) {
    auto function = new Function_With_Block("", input.get_source_range(),
                                            context.get_scope());

    auto function_owner = Function_With_Block_Owner(function);
    auto parameter = new Parameter(parameter_info.get_name(),
                                   Profession_Owner(new Unknown(get_source_point())),
                                   parameter_info.get_source_point());

    function->add_parameter(Parameter_Owner(parameter));
    function->add_parameter(Parameter_Owner(new Parameter(parameter_info.get_name(),
                                                          Profession_Owner(new Unknown(get_source_point())),
                                                          parameter_info.get_source_point())));
    input.next();
    process_expression_block(function->get_block(), context);

    auto lambda = new Lambda(std::move((function_owner)));
    return Expression_Owner(lambda);
  }

  Expression_Owner Expression_Summoner::process_identifier(Context &context) {
    auto path = process_path(context);
    if (input.current().is(lexicon.left_paren)) {
      input.next();
      auto &last = path->get_last();
      if (last.get_type() == Expression_Type::member) {
        auto &member_expression = cast<Member_Expression>(last, Expression_Type::member,
                                                          last.get_name() + " is not a function");
        return process_function_call(path, context);
      }
      else {
        throw std::runtime_error(last.get_name() + " is not a function.");
      }
    }
    else if (input.current().is(lexicon.left_brace)) {
      return process_instantiation(std::move(path), context);
    }
    else if (input.current().is(lexicon.assignment)) {
      auto operator_type = process_assignment_operator(context);
      auto value = process_expression(context);
      return Expression_Owner(new Assignment(path, operator_type, value));
    }
    else {
      return path;
    }
  }

//  void Expression_Summoner::process_constructor_invocation(Member &member) {
//    auto &profession = cast<Profession_Member>(member).get_profession();
//    if (profession.get_type() == Profession_Type::dungeon) {
//      auto &dungeon = cast<Dungeon>(profession);
//      auto function = dungeon.get_function(dungeon.get_name());
//      if (!function) {
//        auto &function2 = dungeon.create_function(dungeon.get_name(),
//                                                  dungeon,
//                                                  input.get_source_point());
//      }
//    }
//  }

  Expression_Owner Expression_Summoner::process_expression(Context &context) {
    auto &token = input.current();
    if (token.is(lexicon.identifier)) {
      auto result = process_identifier(context);
      if (result->get_type() == Expression_Type::member && token.is(lexicon.double_arrow)) {
        auto member = dynamic_cast<Member_Expression *>(result.get());
        return process_lambda(*member, context);
      }
      else {
        return result;
      };
    }

    auto text = token.get_text();
    auto source_point = input.get_source_point();

    if (token.is(lexicon.literal_int)) {
      int value = std::stoi(text);
      input.next();
      return Expression_Owner(new Literal_Int(value, source_point));
    }
    else if (token.is(lexicon.literal_string)) {
      input.next();
      return Expression_Owner(new Literal_String(text, source_point));
    }
    else if (token.is(lexicon.True)) {
      input.next();
      return Expression_Owner(new Literal_Bool(true, source_point));
    }
    else if (token.is(lexicon.False)) {
      input.next();
      return Expression_Owner(new Literal_Bool(false, source_point));
    }
    else {
      throw runic::Syntax_Exception(token, input.get_source_file().get_file_path());
    }
  }

  Operator_Type Expression_Summoner::process_expression_operator(Context &context) {
    Operator_Type result;
    if (!lookup.get_expression_operator(input.next().get_type(), result)) {
      throw runic::Syntax_Exception(input.current(), input.get_source_file().get_file_path());
    }
    return result;
  }

  Operator_Type Expression_Summoner::process_assignment_operator(Context &context) {
    Operator_Type result;
    if (!lookup.get_assignment_operator(input.current().get_type(), result)) {
      throw runic::Syntax_Exception(input.current(), input.get_source_file().get_file_path());
    }
    input.next();
    return result;
  }

  Expression_Owner Expression_Summoner::process_child(Expression_Owner &expression, Context &context) {
//    auto expression = Expression_Owner(new Member_Expression(member));
    if (input.current().is(lexicon.dot)) {
      auto name = input.next().get_text();
      auto child_expression = Expression_Owner(new Member_Expression(name, get_source_point()));
      input.next();
      auto second = process_child(child_expression, context);
      return Expression_Owner(new Chain(expression, second));

//      auto &profession = expression->get_profession();
//      if (profession.get_type() == Profession_Type::unknown) {
//        auto second = Expression_Owner(new Unresolved_Member_Expression(name, input.get_source_point()));
//        return Expression_Owner(new Chain(expression, second));
//      }
//      if (profession.get_type() == Profession_Type::dungeon) {
//        auto &dungeon = *dynamic_cast<const Dungeon *>(&profession);
//        auto member = const_cast<Dungeon &>(dungeon).get_member(name);
//        if (member) {
//          auto child_expression = Expression_Owner(new Member_Expression(*member));
//          auto second = process_child(child_expression, context);
//          return Expression_Owner(new Chain(expression, second));
//        }
//      }

//      throw imp_summoning::Token_Exception(input.current(), "Type " + profession.get_name()
//                                                            + " does not have a member named "
//                                                            + name + ".");
    }
    else {
      return std::move(expression);
    }
  }

  Expression_Owner Expression_Summoner::identify_root(Context &context) {
    if (input.current().get_text() == "this") {
      input.next();
      return Expression_Owner(new Self(context.get_scope().get_dungeon()));
    }
    else {
//      auto &member = find_member(input.current(), context);
      auto result = Expression_Owner(new Member_Expression(input.current().get_text(), get_source_point()));
      input.next();
      return result;
    }
  }

  Expression_Owner Expression_Summoner::process_path(Context &context) {
    auto expression = identify_root(context);
    return process_child(expression, context);
  }

  Expression_Owner Expression_Summoner::process_function_call(Expression_Owner &expression, Context &context) {
    auto source_point = input.get_source_point();

    std::vector<Expression_Owner> arguments;
    while (input.current().is_not(lexicon.right_paren)) {
      arguments.push_back(process_expression(context));
    }

    input.next();
    return Expression_Owner(new underworld::Invoke(expression, arguments, source_point));
  }

  Expression_Owner
  Expression_Summoner::process_instantiation(Expression_Owner profession_expression, Context &context) {
//    auto profession = process_profession(context);
//    if (profession->get_type() == underworld::Profession_Type::unknown)
//      throw runic::Syntax_Exception(input.current());

    auto instantiation = new underworld::Instantiation(std::move(profession_expression), get_source_point());
    Expression_Owner result(instantiation);
    input.next();
    while (!input.current().is(lexicon.right_brace)) {
      auto key = input.expect_next(lexicon.identifier).get_text();
      input.expect_next(lexicon.colon);
      input.next();
      instantiation->add_expression(key, process_expression(context));
    }

    input.next();
    return result;
  }

  Expression_Owner Expression_Summoner::process_statement(Context &context) {
    auto &token = input.current();
    if (token.is(lexicon.Var)) {
      return process_variable_declaration(context);
    }
    else if (token.is(lexicon.If)) {
      return process_if(context);
    }
    else if (token.is(lexicon.Return)) {
      auto &next = input.next();
      if (next.follows_terminator() || next.is(lexicon.right_brace)) {
        return Expression_Owner(new Return());
      }
      else {
        auto value = process_expression(context);
        return Expression_Owner(new Return_With_Value(value));
      }
    }
    else if (token.is(lexicon.identifier)) {
      return process_identifier(context);
//      auto path = process_path(context);
//      if (input.peek().is(lexicon.left_paren)) {
//        return process_function_call(context);
//      }
//      else {
//        auto operator_type = process_assignment_operator(context);
//        auto value = process_expression(context);
//        return Expression_Owner(new Assignment(path, operator_type, value));
//      }
    }
    else {
      throw runic::Syntax_Exception(input.current(), input.get_source_file().get_file_path());
    }
  }

  Expression_Owner Expression_Summoner::process_condition(Context &context) {
    input.expect_next(lexicon.left_paren);
    input.next();
    auto result = process_expression(context);
    input.expect(lexicon.right_paren);
    input.next();
    return result;
  }

  Expression_Owner Expression_Summoner::process_block_or_single_expression(Context &context) {
    if (input.current().is(lexicon.left_brace)) {
      auto block = new Block(context.get_scope());
      Expression_Owner result(block);
      input.next();
      process_block(*block, context);
      return result;
    }
    else {
      return process_statement(context);
    }
  }

  void Expression_Summoner::process_expression_block(underworld::Block &block, Context &context) {
    Child_Context new_context(context, block.get_scope());
    block.add_expression(process_statement(new_context));
  }

  void Expression_Summoner::process_block(underworld::Block &block, Context &context) {
    Child_Context new_context(context, block.get_scope());

//    if (!input.next().is(lexicon.left_brace))
//      throw runic::Expected_Whisper_Exception(input.current(), lexicon.left_brace);

    while (input.current().is_not(lexicon.right_brace)) {
      block.add_expression(process_statement(new_context));
    }

    input.next();
  }

  Expression_Owner Expression_Summoner::process_if(Context &context) {
    auto condition = process_condition(context);
    auto expression = process_block_or_single_expression(context);
    return Expression_Owner(new If(condition, expression));
  }
}
