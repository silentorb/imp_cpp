#include "Summoner.h"
#include "exceptions.h"
#include "Expression_Summoner.h"
#include <underworld/schema/Minion.h>
#include <underworld/expressions/Literal.h>

using namespace std;
using namespace underworld;

namespace summoning {

  Summoner::Summoner(Stream &input, Profession_Library &profession_library) :
    Base_Summoner(input, profession_library) {}

  void Summoner::process_root_identifier(const string &name, Context &context) {
    input.next();
    if (input.current().follows_terminator()) {
      throw Syntax_Exception(input.current());
    }
    else if (input.current().is(lexicon.left_brace)) {
      process_dungeon(name, context);
    }
    else if (input.current().is(lexicon.left_paren)) {
      process_minion(name, context);
    }
    else {
      throw Syntax_Exception(input.current());
    }
  }

  void Summoner::process_dungeon_member(Context &context) {
    if (input.current().is(lexicon.identifier)) {
      process_root_identifier(input.current().get_text(), context);
    }
    else {
      throw Syntax_Exception(input.current());
    }
  }

  void Summoner::process_minion_parameters(underworld::Minion &minion, Context &context) {
    while (!input.next().is(lexicon.right_paren)) {
      throw Syntax_Exception(input.current());
    }
  }

  void Summoner::process_minion(const std::string &name, Context &context) {
    auto &minion = context.get_dungeon().create_minion(name);
    process_minion_parameters(minion, context);

    input.next();
    if (!input.current().is(lexicon.left_brace))
      throw Expected_Whisper_Exception(input.current(), lexicon.left_brace);

    Child_Context new_context(context, minion.get_scope());
    throw "Needs to be moved to Expression_Summoner";
//    process_statements(minion.get_block(), new_context);
  }

  Expression_Owner Summoner::process_expression(Context &context) {
    Expression_Summoner expression_summoner(input, profession_library);
    return expression_summoner.process_expression(context);
  }

  void Summoner::process_dungeon(const std::string &name, Context &context) {
    auto &dungeon = context.get_dungeon().get_or_create_dungeon(name);
    Child_Context new_context(context, dungeon);

    throw "Error";

//    while (require_until(input.current(), lexicon.right_brace)) {
//      process_dungeon_member(input.current(), new_context);
//    }
  }

  void Summoner::process_root(Context &context) {
    throw "Error";
//    while (!input.next().is(lexicon.end_of_file)) {
//      process_dungeon_member(input.current(), context);
//    }
  }

  void Summoner::summon(underworld::Dungeon &root) {
    Root_Context context(root);
    process_root(context);
  }
}