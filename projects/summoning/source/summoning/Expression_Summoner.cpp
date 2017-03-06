#include <underworld/expressions/Literal.h>
#include <underworld/expressions/Flow_Control.h>
#include <underworld/expressions/Minion_Declaration.h>
#include <underworld/expressions/Return.h>
#include "Expression_Summoner.h"
#include "exceptions.h"

using namespace underworld;

namespace summoning {

  Expression_Summoner::Expression_Summoner(Stream &input, Profession_Library &profession_library) :
    Base_Summoner(input, profession_library) {}

  Expression_Owner Expression_Summoner::process_variable_declaration(Context &context) {
    auto &minion = context.get_scope().create_minion(input.expect_next(lexicon.identifier).get_text(),
                                                     profession_library.get_unknown());
    if (input.peek().is(lexicon.assignment)) {
      input.next();
      auto expression = process_expression(context);
      return Expression_Owner(new Minion_Declaration_And_Assignment(minion, expression));
    }
    else {
      return Expression_Owner(new Minion_Declaration(minion));
    }
  }

  Expression_Owner Expression_Summoner::process_expression(Context &context) {
    auto &token = input.next();
    if (token.is(lexicon.literal_int)) {
      int value = std::stoi(token.get_text());
      return Expression_Owner(new Literal_Int(value));
    }
    else if (token.is(lexicon.literal_string)) {
      return Expression_Owner(new Literal_String(token.get_text()));
    }
    else if (token.is(lexicon.True)) {
      return Expression_Owner(new Literal_Bool(true));
    }
    else if (token.is(lexicon.False)) {
      return Expression_Owner(new Literal_Bool(false));
    }
    else {
      throw Syntax_Exception(input.current());
    }
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
    else {
      return process_expression(context);
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
      process_statement(new_context);
    }
  }

  Expression_Owner Expression_Summoner::process_if(Context &context) {
    auto condition = process_condition(context);
    auto expression = process_block_or_single_expression(context);
    return Expression_Owner(new If(condition, expression));
  }
}
