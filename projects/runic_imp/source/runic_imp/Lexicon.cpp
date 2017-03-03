#include "Lexicon.h"

namespace runic_imp {

  static Lexicon instance;

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

  Lexicon &Lexicon::get_instance() {
    return instance;
  }

  Lexicon::Lexicon() : patterns(*(Symbols *) &internal_patterns) {
    add<String_Assignor>(lookup.keywords, internal_patterns.keywords);
    add<String_Assignor>(lookup.double_symbols, internal_patterns.double_symbols);
    add<Char_Assignor>(lookup.single_symbols, internal_patterns.single_symbols);
  }
}