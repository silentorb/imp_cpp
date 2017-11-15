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
    function->add_parameter(Parameter_Owner(new Parameter("",
                                                          Profession_Owner(new Unknown(get_source_point())),
                                                          parameter_info.get_source_point())));
    input.next();
    process_expression_block(function->get_block(), context);

    auto lambda = new Lambda(std::move((function_owner)));
    return Expression_Owner(lambda);
  }

  Expression_Owner create_chain(std::vector<Link> &path) {
    auto chain = new Chain();
    auto result = Expression_Owner(chain);
    for (auto &link: path) {
      chain->add_expression(Expression_Owner(new Member_Expression(link.name, link.source_point)));
    }
    return result;
  }

  underworld::Profession_Owner Expression_Summoner::path_to_profession(const std::vector<Link> &path,
                                                                       Context &context) {
    auto last = process_profession_token(path[path.size() - 1], context);
    for (auto it = path.end() - 1; it >= path.begin(); --it) {
      auto &link = *it;
      last = Profession_Owner(new Dungeon_Reference_Profession(link.name, std::move(last), link.source_point));
    }

    return last;
  }

  Expression_Owner Expression_Summoner::process_identifier(Context &context) {
    std::vector<Link> path;
    process_path(path, context);

    if (input.current().is(lexicon.lesser_than) || input.current().is(lexicon.left_brace)) {
      return process_instantiation(path_to_profession(path, context), context);
    }

    auto chain = create_chain(path);

    if (input.current().is(lexicon.left_paren)) {
      input.next();
      auto &last = chain->get_last();
      if (last.get_type() == Expression_Type::member) {
        auto &member_expression = cast<Member_Expression>(last, Expression_Type::member,
                                                          last.get_name() + " is not a function");
        return process_function_call(chain, context);
      }
      else {
        throw std::runtime_error(last.get_name() + " is not a function.");
      }
    }
    else if (input.current().is(lexicon.assignment)) {
      auto operator_type = process_assignment_operator(context);
      auto value = process_expression(context);
      return Expression_Owner(new Assignment(std::move(chain), operator_type, std::move(value)));
    }
    else {
      return chain;
    }
  }

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

  Expression_Owner Expression_Summoner::identify_root(Context &context) {
    if (input.current().get_text() == "this") {
      input.next();
      return Expression_Owner(new Self(context.get_scope().get_dungeon()));
    }
    else {
      auto result = Expression_Owner(new Member_Expression(input.current().get_text(), get_source_point()));
      input.next();
      return result;
    }
  }

  void Expression_Summoner::process_path(std::vector<Link> &path, Context &context) {
    path.push_back({input.current().get_text(), get_source_point()});
    input.next();

    while (input.current().is(lexicon.dot)) {
      input.expect_next(lexicon.identifier);
      auto name = input.current().get_text();
      input.next();
      path.push_back({name, get_source_point()});
    }
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
  Expression_Summoner::process_instantiation(Profession_Owner profession_expression, Context &context) {
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
