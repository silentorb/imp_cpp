#include <runic_cpp/matching.h>
#include "Imp_Lexer.h"

namespace runic_imp {

  static const char End_Of_File = 0;

  class Premature_End_Of_File : public std::runtime_error {
  public:
      Premature_End_Of_File() : runtime_error("Premature end of file during lexing.") {}
  };

  Imp_Lexer::Imp_Lexer(std::unique_ptr<runic::Text_Source<Char>> &source) :
    lexer(source),
    lexicon(Lexicon::get_instance()) {}

  Imp_Lexer::Imp_Lexer(runic::Text_Source<Char> *source) :
    lexer(source),
    lexicon(Lexicon::get_instance()) {}

  bool is_whitespace_or_semicolon(char value) {
    return runic_cpp::matching::is_whitespace(value) || value == ';';
  }

  bool Imp_Lexer::match_whitespace(Match &result) {
    Char value = lexer.get_character();
    std::string text(1, value);
    bool is_terminator = value == '\n' || value == ';';

    while (is_whitespace_or_semicolon(value = lexer.next_character())) {
      text += value;

      // This could be optimized with more flow control to not check this once is_terminator is true
      if (value == '\n' || value == ';')
        is_terminator = true;
    }

    if (is_terminator) {
      result.set_type(lexicon.patterns.terminator);
      return true;
    }
    else {
      return false;
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

  bool Imp_Lexer::match_special_symbols(Match &result) {
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

  bool Imp_Lexer::match_identifier(Match &result) {
    std::string text(1, lexer.get_character());
    Char value;

    while (runic_cpp::matching::is_identifier_continuing(value = lexer.next_character())) {
      text += value;
    }

    if (lookup(lexicon.lookup.keywords, text, result))
      return true;

    result.set_type(lexicon.patterns.identifier);
    result.set_text(text);
    return true;
  }

  bool Imp_Lexer::match_number(Match &result) {
    std::string text(1, lexer.get_character());
    Char value;
    bool has_dot = false;

    while (runic_cpp::matching::is_number_continuing(value = lexer.next_character())) {
      text += value;
      if (value == '.') {
        if (has_dot)
          return false;

        has_dot = true;
      }
      if (value == 'f') {
        result.set_text(text);
        return true;
      }
    }

    if (has_dot) {
      result.set_type(lexicon.patterns.literal_float);
    }
    else {
      result.set_type(lexicon.patterns.literal_int);
    }

    result.set_text(text);
    return true;
  }

  bool is_string_continuing(char value, char last_value) {
    if (value == End_Of_File)
      throw Premature_End_Of_File();

    return value != '"' || last_value == '\\';
  }

  bool Imp_Lexer::match_string(Match &result) {
    std::string text;
    Char value = '"';

    while (is_string_continuing(lexer.next_character(), value)) {
      value = lexer.get_character();
      text += value;
    }

    result.set_type(lexicon.patterns.literal_string);
    result.set_text(text);

    lexer.next_character(); // Consume closing quotation mark.
    return true;
  }

  bool is_comment_continuing(char value) {
    return value != '\n' && value != End_Of_File;
  }

  bool Imp_Lexer::match_comment(Match &result) {
    while (lexer.next_character()) {
    }
    // Since aside for EOF, comments rely on newlines for scope, I can fall back to returning terminating whitespace.
    // Otherwise this part of the control flow would need to wrap back around to the start of matching anything.
    if (lexer.get_character() != End_Of_File)
      return match_whitespace(result);

    return false;
  }

  bool Imp_Lexer::match_comment_or_division(Match &result) {
    if (lexer.next_character() == '/') {
      return match_comment(result);
    }
    else if (lexer.get_character() == '=') {
      result.set_type(lexicon.patterns.divide_equals);
    }
    else {
      result.set_type(lexicon.patterns.divide);
    }

    return true;
  }

  bool Imp_Lexer::match_non_whitespace(Match &result) {
    auto &value = lexer.get_character();

    if (runic_cpp::matching::is_identifier_start(value))
      return match_identifier(result);

    if (runic_cpp::matching::is_number_start(value))
      return match_identifier(result);

    if (value == '/')
      return match_comment_or_division(result);

    if (value == '"')
      return match_string(result);

    if (match_special_symbols(result))
      return true;

    return false;
  }

  bool Imp_Lexer::match_any(Match &result) {
    auto &value = lexer.get_character();

    if (value == End_Of_File)
      return false;

    if (match_non_whitespace(result))
      return true;

    if (is_whitespace_or_semicolon(value)) {
      if (match_whitespace(result))
        return true;

      return match_non_whitespace(result);
    }

    throw std::runtime_error("Bad syntax at " + lexer.get_position().get_string() + ".");
  }

  bool Imp_Lexer::next_token(Token &token) {
    token.get_range().set_start(lexer.get_position());
    auto &result = token.get_match();
    if (!match_any(result))
      return false;

    token.get_range().set_end(lexer.get_position());

    if (lexer.get_position().get_index() == token.get_range().get_start().get_index())
      lexer.next_character();

    return true;
  }

  void Imp_Lexer::get_all_tokens(std::vector<Token> &tokens) {
    Token token;
    while (next_token(token)) {
      tokens.push_back(token);
    }
  }
}