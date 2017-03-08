#include "Summoner.h"
#include "exceptions.h"
#include "Expression_Summoner.h"
#include <underworld/schema/Function.h>
#include <underworld/expressions/Literal.h>

using namespace std;
using namespace underworld;

namespace summoning {

  Summoner::Summoner(Stream &input, Profession_Library &profession_library) :
    Base_Summoner(input, profession_library),
    expression_summoner(input, profession_library) {}

  void Summoner::process_root_identifier(const string &name, Context &context) {
    input.next();
    if (input.current().follows_terminator()) {
      throw Syntax_Exception(input.current());
    }
    else if (input.current().is(lexicon.left_brace)) {
      process_dungeon(name, context);
    }
    else if (input.current().is(lexicon.left_paren)) {
      process_function(name, context);
    }
    else {
      throw Syntax_Exception(input.current());
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

  void Summoner::process_minion_parameters(Context &context) {
    while (!input.next().is(lexicon.right_paren)) {
      throw Syntax_Exception(input.current());
    }
  }

  void Summoner::process_function(const std::string &name, Context &context) {
    process_minion_parameters(context);
    auto &function = context.get_dungeon().create_function(name);
    expression_summoner.process_block(function.get_block(), context);

//    if (!input.current().is(lexicon.left_brace))
//      throw Expected_Whisper_Exception(input.current(), lexicon.left_brace);

  }

  void Summoner::process_dungeon(const std::string &name, Context &context) {
    auto &dungeon = context.get_dungeon().get_or_create_dungeon(name);
    Child_Context new_context(context, dungeon);

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