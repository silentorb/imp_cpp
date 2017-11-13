#include "Summoner.h"
#include "runic/common/exceptions.h"
#include "Expression_Summoner.h"
#include <underworld/schema/Function.h>
#include <underworld/expressions/Literal.h>
#include "Profession_Finder.h"

using namespace std;
using namespace underworld;

namespace imp_summoning {

  Summoner::Summoner(runic::Stream &input, Parser_Lookup &lookup) :
    Base_Summoner(input, lookup),
    expression_summoner(input, lookup) {}

  Profession_Owner Summoner::process_optional_profession(Context &context) {
    if (input.if_is(lexicon.colon)) {
      return process_profession(context);
    }
    else {
      return Profession_Owner();
    }
  }

  Minion &Summoner::process_minion(Identifier &identifier, Context &context) {
    auto &minion = context.get_scope().create_minion(identifier.name, identifier.source_point);
    process_member_enchantments(minion, identifier, context);
//    if (input.current().is(lexicon.assignment)) {
//
//    }
    return minion;
  }

  void Summoner::process_root_identifier(Identifier &identifier, Context &context) {
    if (input.if_is(lexicon.left_brace)) {
      process_dungeon(identifier, context);
    }
    else {
      input.next();
      process_member(identifier, context, false);
    }
  }

  void Summoner::process_dungeon_with_contracts(Identifier &identifier,
                                                underworld::Profession_Owner first_contract,
                                                Context &context) {
    std::vector<Profession_Owner> contracts;
    do {
      auto contract = process_profession(context);
      contracts.push_back(std::move(contract));
    } while (input.next().is(lexicon.comma));
    auto &dungeon = process_dungeon(identifier, context);
    dungeon.add_contract(std::move(first_contract));
    for (auto &contract: contracts) {
      dungeon.add_contract(std::move(contract));
    }
  }

  void Summoner::process_minion_or_dungeon(Identifier &identifier, Context &context) {
    if (input.if_is(lexicon.colon)) {

      auto profession = process_profession(context);
      if (input.current().is(lexicon.comma)) {
        process_dungeon_with_contracts(identifier, std::move(profession), context);
        return;
      }

      if (input.current().is(lexicon.left_brace)) {
        auto &dungeon = process_dungeon(identifier, context);
        dungeon.add_contract(std::move(profession));
      }
      else {
        auto &minion = process_minion(identifier, context);
        minion.set_profession(std::move(profession));
      }
    }
    else {
      if (input.current().is(lexicon.left_brace)) {
        auto &dungeon = process_dungeon(identifier, context);
      }
      else {
        auto &minion = process_minion(identifier, context);
      }
    }
  }

  std::vector<Generic_Parameter> Summoner::process_generic_parameters(Context &context) {
    std::vector<Generic_Parameter> parameters;
    while (true) {
      auto text = input.expect_next(lexicon.identifier).get_text();
      parameters.push_back({text, input.get_source_range()});

      if (input.next().is_not(lexicon.comma))
        break;
    }

    input.expect(lexicon.greater_than);
    input.next();

    return parameters;
  }

  void Summoner::process_generic_member(Identifier &identifier, Context &context, bool is_static) {
    auto parameters = process_generic_parameters(context);
    if (input.current().is(lexicon.left_paren)) {
      auto &function = process_function(identifier, context);
      for (auto parameter : parameters) {
        function.add_generic_parameter(parameter);
      }
    }
    else {
      if (is_static)
        throw runic::Token_Exception<runic::Token>(input.current(), "Dungeons are already static.");

      auto &dungeon = process_dungeon(identifier, context);
      for (auto parameter : parameters) {
        dungeon.add_generic_parameter({parameter});
      }
    }
  }

  void Summoner::process_member(Identifier &identifier, Context &context, bool is_static) {
    if (input.current().is(lexicon.lesser_than)) {
      process_generic_member(identifier, context, is_static);
    }
    else if (input.current().is(lexicon.left_paren)) {
      auto &function = process_function(identifier, context);
    }
    else {
//      if (is_static) {
//        auto &minion = process_minion(identifier, context);
//        minion.set_is_static(true);
//      }
//      else {
      process_minion_or_dungeon(identifier, context);
//      }
    }
  }

  void Summoner::process_enchantments(underworld::Enchantment_Array &enchantments, Context &context) {
    while (input.current().is(lexicon.at_sign)) {
      input.expect_next(lexicon.identifier);
      auto profession = process_profession(context);
//      auto name = input.expect_next(lexicon.identifier).get_text();
//      input.next();
      auto enchantment = Enchantment_Owner(new Enchantment(std::move(profession)));
      if (input.if_is(lexicon.left_paren)) {
        while (input.current().is_not(lexicon.right_paren)) {
          enchantment->add_argument(expression_summoner.process_expression(context));
          if (input.if_is(lexicon.comma))
            input.next();
          else
            break;
        }
        input.expect(lexicon.right_paren);
        input.next();
      }
      if (input.if_is(lexicon.colon)) {
        context.add_enchantment(enchantment);
      }
      else {
        enchantments.push_back(enchantment);
      }
    }
  }

  void Summoner::process_dungeon_member(Context &context) {
    Identifier identifier;

    process_enchantments(identifier.enchantments, context);

    if (input.current().is(lexicon.identifier)) {
      identifier.name = input.current().get_text();
      identifier.source_point = get_source_point();
      process_root_identifier(identifier, context);
    }
    else {
      throw runic::Syntax_Exception(input.current(), input.get_source_file().get_file_path());
    }
  }

  void Summoner::process_function_parameters(Context &context, vector<Parameter_Owner> &parameters) {
    input.if_is(lexicon.left_paren);
    while (input.current().is_not(lexicon.right_paren) && input.current().is_not(lexicon.end_of_file)) {
//     throw Syntax_Exception(input.current());
      underworld::Enchantment_Array enchantments;
      if (input.current().is(lexicon.at_sign)) {
        process_enchantments(enchantments, context);
      }
      auto name = input.expect(lexicon.identifier).get_text();
      auto source_point = input.get_source_point();
      input.next();
      auto profession = process_optional_profession(context);
      auto parameter = new Parameter(name, std::move(profession), source_point);
      parameters.push_back(Parameter_Owner(parameter));
      for (auto &enchantment : enchantments) {
        parameter->add_enchantment(enchantment);
      }
    }
    input.next();
  }

  Function &Summoner::process_function_internal(Identifier &identifier, Context &context) {
    auto profession = process_optional_profession(context);
    std::vector<Parameter_Owner> parameters;
    process_function_parameters(context, parameters);
    auto return_type_source_range = profession
                                    ? profession->get_source_point()
                                    : source_mapping::Source_Range(input.get_source_point());

    if (input.if_is(lexicon.left_brace)) {
      auto function = new Function_With_Block(identifier.name, identifier.source_point, context.get_scope());
      context.get_scope().add_member(std::unique_ptr<Member>(function));
      function->add_parameters(parameters);
      function->add_parameter(Parameter_Owner(new Parameter("", std::move(profession), return_type_source_range)));

      expression_summoner.process_block(function->get_block(), context);
      return *function;
    }
    else {
      auto function = new Virtual_Function(identifier.name, identifier.source_point, context.get_scope());
      context.get_scope().add_member(std::unique_ptr<Member>(function));
      function->add_parameters(parameters);
      function->add_parameter(Parameter_Owner(new Parameter("", std::move(profession), return_type_source_range)));

      return *function;
    }
  }

  void Summoner::process_member_enchantments(underworld::Member &member, Identifier &identifier, Context &context) {
    for (auto &enchantment : identifier.enchantments) {
      member.add_enchantment(std::move(enchantment));
    }
    for (auto &enchantment : context.get_enchantments()) {
      member.add_enchantment(enchantment);
    }
  }

  Function &Summoner::process_function(Identifier &identifier, Context &context) {
    auto &function = process_function_internal(identifier, context);
    process_member_enchantments(function, identifier, context);
    return function;
  }

  void Summoner::process_dungeon_body(underworld::Dungeon &dungeon, Context &context) {
    Child_Context new_context(context, dungeon);
    while (input.current().is_not(lexicon.right_brace)) {
      process_dungeon_member(new_context);
    }
    input.next();
  }

  Dungeon &Summoner::process_dungeon(Identifier &identifier, Context &context) {
    input.next();
    auto existing_member = context.get_scope().get_member(identifier.name);
    if (existing_member) {
      auto &profession = static_cast<Profession_Member *>(existing_member)->get_profession();
      auto dungeon = dynamic_cast<Dungeon *>(&profession);
      process_dungeon_body(*dungeon, context);
      return *dungeon;
    }
    else {
      auto dungeon = new Dungeon(identifier.name, &context.get_scope(), identifier.source_point);
      for (auto &enchantment : identifier.enchantments) {
        dungeon->add_enchantment(enchantment);
      }
      for (auto &enchantment : context.get_enchantments()) {
        dungeon->add_enchantment(enchantment);
      }

      auto profession = std::unique_ptr<Profession>(dungeon);
      context.get_scope().add_profession(std::move(profession), identifier.source_point);
      process_dungeon_body(*dungeon, context);
      return *dungeon;
    }
  }

  void Summoner::process_root(Context &context) {
    while (input.current().is_not(lexicon.end_of_file)) {
      process_dungeon_member(context);
    }
  }

  void Summoner::summon(underworld::Dungeon &root) {
    Root_Context context(root);
    input.next();
    process_root(context);
  }
}