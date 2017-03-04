#include "Summoner.h"
#include "exceptions.h"
#include <underworld/schema/Minion.h>

using namespace std;

namespace summoning {

  Summoner::Summoner(runic_imp::Imp_Lexer &lexer, underworld::Profession_Library &profession_library) :
    lexer(lexer),
    lexicon(runic_imp::Lexicon::get_instance().patterns),
    profession_library(profession_library) {}

  void Summoner::require_next(Token &token) {
    if (!lexer.next_token(token))
      throw End_Of_File_Exception(token);
  }

  bool Summoner::require_until(Token &token, const runic_imp::Whisper &whisper) {
    if (!lexer.next_token(token))
      throw Expected_Whisper_Exception(whisper, token);

    return !token.is(whisper);
  }

  void Summoner::expect(Token &token, const runic_imp::Whisper &whisper) {
    if (!lexer.next_token(token) || !token.is(whisper))
      throw Expected_Whisper_Exception(whisper, token);
  }

  void Summoner::process_root_identifier(const string &name, Context &context) {
    Token token;
    if (!get_next(token) || token.follows_terminator()) {
      throw Syntax_Exception(token);
    }
    else if (token.is(lexicon.left_brace)) {
      process_dungeon(name, context);
    }
    else if (token.is(lexicon.left_paren)) {
      process_minion(name, context);
    }
    else {
      throw Syntax_Exception(token);
    }
  }

  void Summoner::process_dungeon_member(Token &token, Context &context) {
    if (token.is(lexicon.identifier)) {
      process_root_identifier(token.get_text(), context);
    }
    else {
      throw Syntax_Exception(token);
    }
  }

  void Summoner::process_minion_parameters(underworld::Minion &minion, Context &context) {
    Token token;
    while (get_next(token) && !token.is(lexicon.right_paren)) {
      throw Syntax_Exception(token);
    }
  }

  void Summoner::process_minion(const std::string &name, Context &context) {
    auto &minion = context.get_dungeon().create_minion(name);
    process_minion_parameters(minion, context);

    Token token;
    get_next(token);
    if (!token.is(lexicon.left_brace))
      throw Expected_Whisper_Exception(lexicon.left_brace, token);

    Child_Context new_context(context, minion.get_scope());
    process_statements(minion.get_block(), new_context);
  }

  void Summoner::process_expression(Context &context) {
    Token token;
    require_next(token);
  }

  void Summoner::process_variable_declaration(Context &context) {
    Token token;
    expect(token, lexicon.identifier);
    context.get_scope().create_portal(token.get_text(), profession_library.get_unknown());
    if (get_next(token) && token.is(lexicon.assignment)) {
      process_expression(context);
    }
  }

  void Summoner::process_statements(underworld::Block &block, Context &context) {
    Token token;
    require_next(token);

    while (!token.is(lexicon.right_brace)) {
      if (token.is(lexicon.Var)) {
        process_variable_declaration(context);
      }
      else {
        throw Syntax_Exception(token);
      }
    }
  }

  void Summoner::process_dungeon(const std::string &name, Context &context) {
    auto &dungeon = context.get_dungeon().get_or_create_dungeon(name);
    Child_Context new_context(context, dungeon);

    Token token;
    while (require_until(token, lexicon.right_brace)) {
      process_dungeon_member(token, new_context);
    }
  }

  void Summoner::process_root(Context &context) {
    Token token;
    while (get_next(token)) {
      process_dungeon_member(token, context);
    }
  }

  void Summoner::summon(underworld::Dungeon &root) {
    Root_Context context(root);
    process_root(context);
  }
}