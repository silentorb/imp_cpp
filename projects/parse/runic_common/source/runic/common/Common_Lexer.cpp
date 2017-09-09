#include <runic/matching.h>
#include "Common_Lexer.h"

using namespace std;

namespace runic {

  static const char End_Of_File = 0;

  class Premature_End_Of_File : public std::runtime_error {
  public:
      Premature_End_Of_File() : runtime_error("Premature end of file during lexing.") {}
  };

  Common_Lexer::Common_Lexer(std::unique_ptr<runic::Text_Source<Char>> source) :
    lexer(source),
    lexicon(runic::Common_Lexicon::get_instance()) {}

//  Common_Lexer::Common_Lexer(runic::Text_Source<Char> *source) :
//    lexer(source),
//    lexicon(runic::Common_Lexicon::get_instance()) {}

  bool is_whitespace_or_semicolon(char value) {
    return runic::matching::is_whitespace(value) || value == ';';
  }

  void Common_Lexer::consume_whitespace(bool &follows_whitespace) {
    Char value = lexer.get_character();
    if (value == '\n' || value == ';')
      follows_whitespace = true;

    while (is_whitespace_or_semicolon(value = lexer.next_character())) {
      // This could be optimized with more flow control to not check this once is_terminator is true
      if (value == '\n' || value == ';')
        follows_whitespace = true;
      else if (value == End_Of_File)
        return;
    }
  }

  template<typename T>
  bool lookup(const std::map<T, const Whisper *> &dictionary, const T &text, Match &result) {
    auto search = dictionary.find(text);
    if (search != dictionary.end()) {
      result.set_type((*search).second);
      return true;
    }

    return false;
  }

  bool Common_Lexer::match_special_symbols(Match &result) {
    auto &first = lexer.get_character();
    if (lookup(lexicon.lookup.single_symbols, first, result)) {
      const char data[3] = {first, lexer.next_character(), 0};
      lookup(lexicon.lookup.double_symbols, std::string(data), result);
      return true;
    }
    else if (first == '!' && lexer.next_character() == '=') {
      return true;
    }

    return false;
  }

  void Common_Lexer::match_identifier(Match &result) {
//    token.get_range().set_start(lexer.get_position());
    std::string text(1, lexer.get_character());
    Char value;

    while (runic::matching::is_identifier_continuing(value = lexer.next_character())) {
      text += value;
    }

    if (lookup(lexicon.lookup.keywords, text, result))
      return;

    result.set_type(lexicon.patterns.identifier);
    result.set_text(text);
  }

  void Common_Lexer::match_number(Match &result) {
    std::string text(1, lexer.get_character());
    Char value;
    bool has_dot = false;

    while (runic::matching::is_number_continuing(value = lexer.next_character())) {
      text += value;
      if (value == '.') {
        if (has_dot)
          throw runtime_error("Number can only have one dot.");

        has_dot = true;
      }
      if (value == 'f') {
        if (runic::matching::is_identifier_continuing(lexer.next_character()))
          throw runtime_error("Invalid numeric character.");

        result.set_type(lexicon.patterns.literal_float);
        result.set_text(text);
        return;
      }
    }

    if (has_dot) {
      result.set_type(lexicon.patterns.literal_double);
    }
    else {
      result.set_type(lexicon.patterns.literal_int);
    }

    result.set_text(text);
  }

  bool is_string_continuing(char value, char last_value) {
    if (value == End_Of_File)
      throw Premature_End_Of_File();

    return value != '"' || last_value == '\\';
  }

  void Common_Lexer::match_string(Match &result) {
    std::string text;
    Char value = '"';

    while (is_string_continuing(lexer.next_character(), value)) {
      value = lexer.get_character();
      text += value;
    }

    result.set_type(lexicon.patterns.literal_string);
    result.set_text(text);

    lexer.next_character(); // Consume closing quotation mark.
  }

  bool is_comment_continuing(char value) {
    return value != '\n' && value != End_Of_File;
  }

  void Common_Lexer::consume_to_end_of_line() {
    while (lexer.next_character() != End_Of_File && lexer.get_character() != '\n');
  }

  Token_Result Common_Lexer::match_comment_or_division(Match &result) {
    if (lexer.next_character() == '/') {
      consume_to_end_of_line();
      return Token_Result::loop;
    }
    else if (lexer.get_character() == '=') {
      result.set_type(lexicon.patterns.divide_equals);
    }
    else {
      result.set_type(lexicon.patterns.divide);
    }

    return Token_Result::token;
  }

  Token_Result Common_Lexer::match_non_whitespace(Match &result) {
    auto &value = lexer.get_character();

    if (runic::matching::is_identifier_start(value)) {
      match_identifier(result);
    }
    else if (runic::matching::is_number_start(value)) {
      match_number(result);
    }
    else if (value == '/') {
      return match_comment_or_division(result);
    }
    else if (value == '"') {
      match_string(result);
    }
    else if (match_special_symbols(result)) {
    }
    else {
      throw std::runtime_error("Bad syntax at " + lexer.get_position().get_string() + ".");
    }

    return Token_Result::token;
  }

  bool Common_Lexer::match_any(Match &result, Token &token, bool &follows_whitespace) {
    do {
      auto &value = lexer.get_character();

      if (is_whitespace_or_semicolon(value))
        consume_whitespace(follows_whitespace);

      token.get_match().set_text("");
      token.get_range().set_start(lexer.get_position());

      if (value == End_Of_File || value == 1)
        return false;

    } while (match_non_whitespace(result) == Token_Result::loop);

    return true;
  }

  bool Common_Lexer::next_token(Token &token) {
    bool follows_whitespace = false;
    auto &result = token.get_match();
    if (!match_any(result, token, follows_whitespace)) {
      token.get_match().set_type(lexicon.patterns.end_of_file);
      return false;
    }

    token.get_range().set_end(lexer.get_position());
    token.set_follows_terminator(follows_whitespace);

    if (lexer.get_position().get_index() == token.get_range().get_start().get_index())
      lexer.next_character();

    return true;
  }

  void Common_Lexer::get_all_tokens(std::vector<Token> &tokens) {
    Token token;
    while (next_token(token)) {
      tokens.push_back(token);
    }
  }
}