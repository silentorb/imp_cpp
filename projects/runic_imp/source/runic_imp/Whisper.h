#pragma once

namespace runic_imp {

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

//      void set_name(const std::string &value) {
//        name = value;
//      }
  };
}