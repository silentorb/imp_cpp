#pragma once
#include <string>

namespace runic_cpp {

  class Whisper {
      std::string name;

  public:
      Whisper(const std::string &name) :
        name(name) {}

      Whisper(const char *name) :
        name(name) {}

      const std::string &get_name() const {
        return name;
      }
  };
}