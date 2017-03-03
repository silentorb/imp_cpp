#pragma once

#include <string>

namespace runic {

  using Position_Value = unsigned long;

  class Position {
      Position_Value index = 0;
      Position_Value row = 1;
      Position_Value column = 1;

  public:

      void step() {
        ++index;
        ++column;
      }

      void newline() {
        ++index;
        ++row;
        column = 1;
      }

      Position_Value get_index() const {
        return index;
      }

      Position_Value get_row() const {
        return row;
      }

      Position_Value get_column() const {
        return column;
      }

      const std::string get_string() const {
        return std::to_string(row) + ":" + std::to_string(column);
      }
  };
}