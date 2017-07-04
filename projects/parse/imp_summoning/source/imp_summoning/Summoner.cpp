#include "Summoner.h"
#include "exceptions.h"
#include "Expression_Summoner.h"
#include <underworld/schema/Function.h>
#include <underworld/expressions/Literal.h>
#include "Profession_Finder.h"

using namespace std;
using namespace underworld;

namespace imp_summoning {

  Summoner::Summoner(Stream &input, Lookup &lookup) :
    Base_Summoner(input, lookup),
    expression_summoner(input, lookup) {}

  underworld::Profession_Owner Summoner::process_profession(Context &context) {
    auto primitive = lookup.get_primitive(input.current().get_match().get_type());
    if (primitive != Primitive_Type::Unknown)
      return Profession_Owner(new Primitive(primitive));

    auto profession = find_profession(context.get_scope(), lexicon, input);
    if (profession)
      return profession;

    throw Syntax_Exception(input.current());
  }

  Profession_Owner Summoner::process_optional_profession(Context &context) {
    if (input.peek().is(lexicon.colon)) {
      input.next();
      input.next();
      return process_profession(context);
    }
    else {
      return Profession_Owner();
    }
  }

  void Summoner::process_minion(const std::string &name, Context &context) {
    auto profession = process_optional_profession(context);
    context.get_scope().create_minion(name, profession, input.get_source_point());
  }

  void Summoner::process_root_identifier(const string &name, Context &context) {
    if (input.peek().is(lexicon.left_brace)) {
      if (input.next().follows_terminator())
        throw Syntax_Exception(input.current());

      process_dungeon(name, context);
    }
    else if (input.peek().is(lexicon.left_paren)) {
      if (input.next().follows_terminator())
        throw Syntax_Exception(input.current());

      process_function(name, context);
    }
    else {
      process_minion(name, context);
//      throw Syntax_Exception(input.current());
    }
  }

  void Summoner::process_dungeon_member(Context &context) {
    if (input.current().is(lexicon.identifier)) {
      // Make sure to copy the name before the token gets deleted.
      auto name = input.current().get_text();
      process_root_identifier(name, context);
    }
    else {
      throw Syntax_Exception(input.current());
    }
  }

  void Summoner::process_function_parameters(Context &context, Function &func) {
    while (!input.next().is(lexicon.right_paren)) {
//     throw Syntax_Exception(input.current());
      auto source_point = input.get_source_point();
      auto &name = input.current().get_text();
      auto profession = process_optional_profession(context);
      auto &minion = func.add_parameter(name, profession, source_point);
    }
//    input.next();
  }

  void Summoner::process_function(const std::string &name, Context &context) {
    auto profession = process_optional_profession(context);
    auto &function = context.get_scope().create_function(name, profession, input.get_source_point());
    process_function_parameters(context, function);
//    input.current().get_text();
//    input.next();
    expression_summoner.process_block(function.get_block(), context);

//    if (!input.current().is(lexicon.left_brace))
//      throw Expected_Whisper_Exception(input.current(), lexicon.left_brace);

  }

  void Summoner::process_dungeon(const std::string &name, Context &context) {
    auto dungeon = std::unique_ptr<Profession>(new Dungeon(name, &context.get_scope()));
//    auto &dungeon = context.get_dungeon().get_or_create_dungeon(name);
    Child_Context new_context(context, *dynamic_cast<Dungeon *>(dungeon.get()));
    context.get_scope().add_profession(dungeon, input.get_source_point());
    while (input.next().is_not(lexicon.right_brace)) {
      process_dungeon_member(new_context);
    }
  }

  void Summoner::process_root(Context &context) {
    while (input.next().is_not(lexicon.end_of_file)) {
      process_dungeon_member(context);
    }
  }

  void Summoner::summon(underworld::Dungeon &root) {
    Root_Context context(root);
    process_root(context);
  }
}