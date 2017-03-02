#pragma once

#include <fstream>
#include "Text_Source.h"

namespace runic {

  template<typename Character>
  class File_Text_Source : public virtual Text_Source<Character> {
      std::fstream file_stream;

  public:
      File_Text_Source(const std::string &file_path)
        : file_stream(file_path, std::fstream::in) {}

      Character next_character() override {
        Character result;
        file_stream >> std::noskipws >> result;
        return result;
      }
  };
}