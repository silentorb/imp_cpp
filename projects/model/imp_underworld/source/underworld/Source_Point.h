#pragma once

#include <string>

namespace underworld {

  class Source_File {
      const std::string file_path;

  public:
      Source_File(const std::string &file_path) : file_path(file_path) {}

      const std::string &get_file_path() {
        return file_path;
      }
  };

  class Source_Point {
      Source_File &source_file;
      int column;
      int row;

  public:
      Source_File &get_source_file() const {
        return source_file;
      }

      int get_column() const {
        return column;
      }

      int get_row() const {
        return row;
      }
  };
}