#pragma once

#include <map>
#include "Whisper.h"
#include <set>

namespace runic {

  using Whisper_Dictionary = std::map<std::string, const Whisper *>;
  using Whisper_Dictionary_Char = std::map<char, const Whisper *>;

  struct Keywords {
      Whisper Break = "break";
      Whisper Continue = "continue";
      Whisper If = "if";
      Whisper False = "false";
      Whisper For = "for";
//      Whisper New = "new";
      Whisper Return = "return";
      Whisper True = "true";
      Whisper Var = "var";

      Whisper Bool = "bool";
      Whisper Double = "double";
      Whisper Int = "int";
      Whisper Float = "float";
      Whisper String = "string";

      Whisper Static = "static";
  };

  struct Single_Symbols {
      Whisper left_paren = "(";
      Whisper right_paren = ")";
      Whisper left_brace = "{";
      Whisper right_brace = "}";
      Whisper left_bracket = "[";
      Whisper right_bracket = "]";

      Whisper plus = "+";
      Whisper minus = "-";
      Whisper multiply = "*";
      Whisper modulus = "%";

      Whisper assignment = "=";
      Whisper colon = ":";
      Whisper dot = ".";
      Whisper comma = ",";

      Whisper lesser_than = "<";
      Whisper greater_than = ">";

      Whisper bitwise_or = "|";
      Whisper ampersand = "&";
      Whisper at_sign = "@";
  };

  // The way the parser currently works, the first character of each of these whispers must exist in Single_Symbols
  struct Double_Symbols {
      Whisper arrow = "=>";

      Whisper plus_equals = "+=";
      Whisper subtract_equals = "-=";
      Whisper multiply_equals = "*=";
      Whisper modulus_equals = "%=";

      Whisper lesser_than_or_equal = "<=";
      Whisper greater_than_or_equal = ">=";
      Whisper logical_and = "&&";
      Whisper logical_or = "||";
      Whisper check_equals = "==";
  };

  struct Special_Symbols {
      Whisper end_of_file = "EOF";
      Whisper terminator = ";";
      Whisper does_not_equal = "!="; // Since there is no single symbol for exclamation point, this whisper is handled specially.

      // These are specially handled alongside comments because of their overlapping first character.
      Whisper divide = "/";
      Whisper divide_equals = "/=";
  };

  struct Dynamic_Whispers {
      Whisper identifier = "identifier";
      Whisper literal_string = "literal string";
      Whisper literal_double = "literal double";
      Whisper literal_int = "literal int";
      Whisper literal_float = "literal float";
  };

  struct Symbols : Keywords, Double_Symbols, Single_Symbols, Special_Symbols, Dynamic_Whispers {
  };

  struct String_Assignor {
      static void assign(Whisper_Dictionary &dictionary, const Whisper *whisper) {
        dictionary[whisper->get_name()] = whisper;
      }
  };

  struct Char_Assignor {
      static void assign(Whisper_Dictionary_Char &dictionary, const Whisper *whisper) {
        dictionary[whisper->get_name()[0]] = whisper;
      }
  };

  template<typename Assignor, typename T, typename Element>
  void add(std::map<Element, const Whisper *> &dictionary, const T &structure) {
    auto count = sizeof(T) / sizeof(Whisper);
    auto *whisper = ((const Whisper *) &structure);
    auto end = whisper + count;
    while (whisper != end) {
      Assignor::assign(dictionary, whisper);
      ++whisper;
    }
  }

  struct Lookup {
      Whisper_Dictionary keywords;
      Whisper_Dictionary double_symbols;
      Whisper_Dictionary_Char single_symbols;

      template<typename Keywords, typename Double_Symbols, typename Single_Symbols, typename Symbols>
      static void initialize(Lookup &lookup, Symbols &symbols) {
        add<String_Assignor>(lookup.keywords, static_cast<Keywords>(symbols));
        add<String_Assignor>(lookup.double_symbols, static_cast<Double_Symbols>(symbols));
        add<Char_Assignor>(lookup.single_symbols, static_cast<Single_Symbols>(symbols));
      };
  };

  template<typename Symbols>
  struct Lexer_Bundle {
     const Symbols &lexicon;
      Lookup &lookup;

      Lexer_Bundle(const Symbols &lexicon, Lookup &lookup) : lexicon(lexicon), lookup(lookup) {}
  };

//  class Common_Lexicon {
//
//      struct Internal_Symbols {
//          Keywords keywords;
//          Double_Symbols double_symbols;
//          Single_Symbols single_symbols;
//          Special_Symbols special_symbols;
//          Dynamic_Whispers dynamic;
//      };
//
//      Internal_Symbols internal_patterns;
//
//  public:
//      Lookup lookup;
//      const Symbols &patterns;
//
//      Common_Lexicon();
//      static Common_Lexicon &get_instance();
//  };
}