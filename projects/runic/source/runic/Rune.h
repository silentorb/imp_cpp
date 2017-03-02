#pragma once

#include "Whisper.h"
#include <string>

namespace runic {

  class Rune {
      unsigned int index;
      Whisper *type;

  public:
      Rune(unsigned int index, Whisper *type) : index(index), type(type) {}

      unsigned int get_index() {
        return index;
      }
  };

  class String_Rune : Rune {
      std::string text;

  public:
      String_Rune(unsigned int index, Whisper *type, const std::string &text) :
        Rune(index, type), text(text) {}
  };
}