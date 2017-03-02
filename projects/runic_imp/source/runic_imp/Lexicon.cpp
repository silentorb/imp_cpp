#include "Lexicon.h"

namespace runic_imp {

  Lexicon::Lexicon() {
    add_many_keywords(
      {
        {"if", Keyword::If},
        {"new", Keyword::New},
        {"return", Keyword::Return},
        {"true", Keyword::True},
      });
  }

  void Lexicon::add_many_keywords(std::initializer_list<std::pair<std::string, Keyword>> initializer) {
    for (auto &entry : initializer) {
      keywords.insert(entry);
    }
  }
}