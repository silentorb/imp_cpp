#pragma once

#include <runic/Lexer.h>
#include <runic/Rune.h>
#include "Whisper.h"
#include "Common_Lexicon.h"
#include "Token.h"
#include <vector>
#include <runic/matching.h>

namespace runic {

  using Char = char;
  using Match = runic::Generic_Match<runic::Whisper>;

  enum class Token_Result {
      finished,
      token,
      loop,
      error, // Not yet used but might be someday.
  };

  class Premature_End_Of_File : public std::runtime_error {
  public:
      Premature_End_Of_File() : runtime_error("Premature end of file during lexing.") {}
  };

  inline bool is_whitespace_or_semicolon(char value) {
    return runic::matching::is_whitespace(value) || value == ';';
  }

  static const char End_Of_File = 0;

  template<typename T>
  bool lookup_symbol(const std::map<T, const Whisper *> &dictionary, const T &text, Match &result) {
    auto search = dictionary.find(text);
    if (search != dictionary.end()) {
      result.set_type((*search).second);
      return true;
    }

    return false;
  }

  class Lexer_Interface {
  public:
      virtual bool next_token(Token &token) = 0;
  };

  template<typename Symbols>
  class Common_Lexer : public Lexer_Interface {
      runic::Lexer<Char> lexer;
      const Symbols &lexicon;
      Lookup &lookup;

//      void consume_whitespace(bool &follows_whitespace);
//      Token_Result match_non_whitespace(Match &result);
//      void match_identifier(Match &result);
//      void match_number(Match &result);
//      void match_string(Match &result);
//      void consume_to_end_of_line();
//      Token_Result match_comment_or_division(Match &result);
//
//      bool match_special_symbols(Match &result);
//      bool match_any(Match &result, runic::Token &token, bool &follows_whitespace);

      void consume_whitespace(bool &follows_whitespace) {
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

      bool match_special_symbols(Match &result) {
        auto &first = lexer.get_character();
        if (lookup_symbol(lookup.single_symbols, first, result)) {
          const char data[3] = {first, lexer.next_character(), 0};
          lookup_symbol(lookup.double_symbols, std::string(data), result);
          return true;
        }
        else if (first == '!' && lexer.next_character() == '=') {
          return true;
        }

        return false;
      }

      void match_identifier(Match &result) {
//    token.get_range().set_start(lexer.get_position());
        std::string text(1, lexer.get_character());
        Char value;

        while (runic::matching::is_identifier_continuing(value = lexer.next_character())) {
          text += value;
        }

        if (lookup_symbol(lookup.keywords, text, result))
          return;

        result.set_type(lexicon.identifier);
        result.set_text(text);
      }

      void match_number(Match &result) {
        std::string text(1, lexer.get_character());
        Char value;
        bool has_dot = false;

        while (runic::matching::is_number_continuing(value = lexer.next_character())) {
          text += value;
          if (value == '.') {
            if (has_dot)
              throw std::runtime_error("Number can only have one dot.");

            has_dot = true;
          }
          if (value == 'f') {
            if (runic::matching::is_identifier_continuing(lexer.next_character()))
              throw std::runtime_error("Invalid numeric character.");

            result.set_type(lexicon.literal_float);
            result.set_text(text);
            return;
          }
        }

        if (has_dot) {
          result.set_type(lexicon.literal_double);
        }
        else {
          result.set_type(lexicon.literal_int);
        }

        result.set_text(text);
      }

      bool is_string_continuing(char value, char last_value) {
        if (value == End_Of_File)
          throw Premature_End_Of_File();

        return value != '"' || last_value == '\\';
      }

      void match_string(Match &result) {
        std::string text;
        Char value = '"';

        while (is_string_continuing(lexer.next_character(), value)) {
          value = lexer.get_character();
          text += value;
        }

        result.set_type(lexicon.literal_string);
        result.set_text(text);

        lexer.next_character(); // Consume closing quotation mark.
      }

      bool is_comment_continuing(char value) {
        return value != '\n' && value != End_Of_File;
      }

      void consume_to_end_of_line() {
        while (lexer.next_character() != End_Of_File && lexer.get_character() != '\n');
      }

      Token_Result match_comment_or_division(Match &result) {
        if (lexer.next_character() == '/') {
          consume_to_end_of_line();
          return Token_Result::loop;
        }
        else if (lexer.get_character() == '=') {
          result.set_type(lexicon.divide_equals);
        }
        else {
          result.set_type(lexicon.divide);
        }

        return Token_Result::token;
      }

      Token_Result match_non_whitespace(Match &result) {
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

      bool match_any(Match &result, Token &token, bool &follows_whitespace) {
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

  public:
      Common_Lexer(std::unique_ptr<runic::Text_Source<Char>> source, Lexer_Bundle<Symbols> lexer_bundle) :
        lexer(source), lexicon(lexer_bundle.lexicon), lookup(lexer_bundle.lookup) {}

//      bool next_token(runic::Token &token);
//      void get_all_tokens(std::vector<runic::Token> &tokens);

      virtual bool next_token(Token &token) override {
        bool follows_whitespace = false;
        auto &result = token.get_match();
        if (!match_any(result, token, follows_whitespace)) {
          token.get_match().set_type(lexicon.end_of_file);
          return false;
        }

        token.get_range().set_end(lexer.get_position());
        token.set_follows_terminator(follows_whitespace);

        if (lexer.get_position().get_index() == token.get_range().get_start().get_index())
          lexer.next_character();

        return true;
      }

      void get_all_tokens(std::vector<Token> &tokens) {
        Token token;
        while (next_token(token)) {
          tokens.push_back(token);
        }
      }

      const runic::Position &get_position() const {
        return lexer.get_position();
      }

  };
}