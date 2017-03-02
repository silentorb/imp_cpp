#pragma once

#include <map>

namespace runic_imp {

  enum class Keyword {
      If,
      New,
      Return,
      True
  };

  class Lexicon {
      std::map<std::string, Keyword> keywords;

      void add_many_keywords(std::initializer_list<std::pair<std::string, Keyword>> initializer);

  public:
      Lexicon();
  };
}