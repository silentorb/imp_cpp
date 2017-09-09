#pragma once

namespace runic {
  namespace matching {


    // Most of these functions rely on ASCII alignment.  If another character format is used they will break.

    inline bool is_identifier_start(char value) {
      return
        value >= 'A' && value <= 'z' // General range
        && (value <= 'Z' || value >= 'a' || value == '_'); // Specific range
    }

    inline bool is_identifier_continuing(char value) {
      return
        is_identifier_start(value)
        || (value >= '0' && value <= '9');
    }

    // Numbers can also start with a hyphen but that requires context
    // so determining negative numbers is deferred to the Parser.
    inline bool is_number_start(char value) {
      return value >= '0' && value <= '9';
    }

    inline bool is_number_continuing(char value) {
      return is_number_start(value)
             || value == '.' || value == 'f';
    }

    inline bool is_whitespace(char value) {
      return value == 32    // space
             || value == 9       // tab
             || value == '\n'    // newline
             || value == '\r'    // carriage return (Mostly ignored)
        ;
    }

  }
}