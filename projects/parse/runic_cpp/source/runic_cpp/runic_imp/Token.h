#pragma once

#include <runic/Rune.h>
#include "Whisper.h"

namespace runic_cpp {

  class Token : public runic::Rune<Whisper> {
      bool _follows_terminator = false;

  public:
//      Token(runic::Source *source) : runic::Rune<Whisper>(source) {}
//      Token() : runic::Rune<Whisper>(nullptr) {}

      virtual ~Token() {}

      bool follows_terminator() const {
        return _follows_terminator;
      }

      void set_follows_terminator(bool value) {
        _follows_terminator = value;
      }

      const runic::Position &get_start() const {
        return get_range().get_start();
      }

      const std::string get_info_string() const {
        auto &text = get_match().get_text();
        auto type = get_match().get_type();
        return text != ""
               ? text
               : type ? type->get_name() : "";
      }

      const Whisper *get_type() const {
        return get_match().get_type();
      }
  };

  inline std::ostream &operator<<(std::ostream &Str, const Token &v) {
    // print something from v to str, e.g: Str << v.getX();
    return Str << std::string("testing");
  }
}