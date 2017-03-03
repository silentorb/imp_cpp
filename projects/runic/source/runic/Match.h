#pragma once
#include <string>

namespace runic {

  template<typename Whisper>
  class Match {
      const Whisper *type = nullptr;
      std::string text;

  public:
      const Whisper *get_type() const {
        return type;
      }

      void set_type(const Whisper *value) {
        type = value;
      }

      void set_type(const Whisper &value) {
        type = &value;
      }

      const std::string &get_text() const {
        return text;
      }

      void set_text(const std::string &value) {
       text = value;
      }
  };
}