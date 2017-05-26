#include <underworld/expressions/Literal.h>
#include <underworld/expressions/Flow_Control.h>
#include <underworld/expressions/Minion_Declaration.h>
#include <underworld/expressions/Return.h>
#include <underworld/expressions/Assignment.h>
#include <underworld/expressions/Minion_Expression.h>
#include <underworld/expressions/Function_Call.h>
#include "Expression_Summoner.h"
#include "exceptions.h"

using namespace underworld;

namespace imp_summoning {

  Expression_Summoner::Expression_Summoner(Stream &input, Lookup &lookup) :
    Base_Summoner(input, lookup) {}

  Expression_Owner Expression_Summoner::process_variable_declaration(Context &context) {
    auto &minion = context.get_scope().create_minion(input.expect_next(lexicon.identifier).get_text(),
                                                     profession_library.get_unknown(), input.get_source_point());
    if (input.peek().is(lexicon.assignment)) {
      input.next();
      auto expression = process_expression(context);
      return Expression_Owner(new Minion_Declaration_And_Assignment(minion, expression));
    }
    else {
      return Expression_Owner(new Minion_Declaration(minion));
    }
  }

  Expression_Owner Expression_Summoner::process_identifier(Context &context) {
    auto path = process_path(context);
    if (input.peek().is(lexicon.left_paren)) {
      return process_function_call(context);
    }
    else {
      auto operator_type = process_assignment_operator(context);
      auto value = process_expression(context);
      return Expression_Owner(new Assignment(path, operator_type, value));
    }
  }

  Expression_Owner Expression_Summoner::process_expression(Context &context) {
    auto &token = input.next();
    if (token.is(lexicon.literal_int)) {
      int value = std::stoi(token.get_text());
      return Expression_Owner(new Literal_Int(value, input.get_source_point()));
    }
    else if (token.is(lexicon.literal_string)) {
      return Expression_Owner(new Literal_String(token.get_text(), input.get_source_point()));
    }
    else if (token.is(lexicon.True)) {
      return Expression_Owner(new Literal_Bool(true, input.get_source_point()));
    }
    else if (token.is(lexicon.False)) {
      return Expression_Owner(new Literal_Bool(false, input.get_source_point()));
    }
    else if (token.is(lexicon.identifier)) {
      auto path = process_path(context);
      if (input.peek().is(lexicon.left_paren)) {
        return process_function_call(context);
      }
      else {
        return path;
      }
    }
    else {
      throw Syntax_Exception(token);
    }
  }

  Operator_Type Expression_Summoner::process_expression_operator(Context &context) {
    Operator_Type result;
    if (!lookup.get_expression_operator(input.next().get_type(), result)) {
      throw Syntax_Exception(input.current());
    }
    return result;
  }

  Operator_Type Expression_Summoner::process_assignment_operator(Context &context) {
    Operator_Type result;
    if (!lookup.get_assignment_operator(input.next().get_type(), result)) {
      throw Syntax_Exception(input.current());
    }
    return result;
  }

  Expression_Owner Expression_Summoner::process_path(Context &context) {
    auto member = context.find_member(input.current().get_text());
    if (!member)
      throw Syntax_Exception(input.current());

    return Expression_Owner(new Minion_Expression(*dynamic_cast<underworld::Minion *>(member)));
  }

  Expression_Owner Expression_Summoner::process_function_call(Context &context) {
    auto member = context.find_member(input.current().get_text());
    if (!member)
      throw Syntax_Exception(input.current());

    auto source_point = input.get_source_point();

    std::vector<Expression_Owner> arguments;
    while (!input.next().is(lexicon.right_paren)) {
      auto source_point = input.get_source_point();
      arguments.push_back(process_expression(context));
    }

    return Expression_Owner(new underworld::Function_Call(
      *dynamic_cast<underworld::Function *>(member), arguments, source_point
    ));
  }

  Expression_Owner Expression_Summoner::process_statement(Context &context) {
    auto &token = input.next();
    if (token.is(lexicon.Var)) {
      return process_variable_declaration(context);
    }
    else if (token.is(lexicon.If)) {
      return process_if(context);
    }
    else if (token.is(lexicon.Return)) {
      auto &next = input.peek();
      if (next.follows_terminator() || next.is(lexicon.right_brace)) {
        return Expression_Owner(new Return());
      }
      else {
        auto value = process_expression(context);
        return Expression_Owner(new Return_With_Value(value));
      }
    }
    else if (token.is(lexicon.identifier)) {
      auto path = process_path(context);
      if (input.peek().is(lexicon.left_paren)) {
        return process_function_call(context);
      }
      else {
        auto operator_type = process_assignment_operator(context);
        auto value = process_expression(context);
        return Expression_Owner(new Assignment(path, operator_type, value));
      }
    }
    else {
      throw Syntax_Exception(input.current());
    }
  }

  Expression_Owner Expression_Summoner::process_condition(Context &context) {
    input.expect_next(lexicon.left_paren);
    auto result = process_expression(context);
    input.expect_next(lexicon.right_paren);
    return result;
  }

  Expression_Owner Expression_Summoner::process_block_or_single_expression(Context &context) {
    if (input.peek().is(lexicon.left_brace)) {
      auto block = new Block();
      Expression_Owner result(block);
      process_block(*block, context);
      return result;
    }
    else {
      return process_statement(context);
    }
  }

  void Expression_Summoner::process_block(underworld::Block &block, Context &context) {
    Child_Context new_context(context, block.get_scope());

    if (!input.next().is(lexicon.left_brace))
      throw Expected_Whisper_Exception(input.current(), lexicon.left_brace);

    while (input.until(lexicon.right_brace)) {
      block.add_expression(process_statement(new_context));
    }
  }

  Expression_Owner Expression_Summoner::process_if(Context &context) {
    auto condition = process_condition(context);
    auto expression = process_block_or_single_expression(context);
    return Expression_Owner(new If(condition, expression));
  }
}
