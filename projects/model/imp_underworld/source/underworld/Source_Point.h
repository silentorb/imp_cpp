#pragma once

#include <string>

namespace underworld {

  class Source_File {
      const std::string file_path;

  public:
      Source_File(const std::string &file_path) : file_path(file_path) {}

      const std::string &get_file_path() const {
        return file_path;
      }
  };

  class Source_Point {
      const Source_File *source_file;
      unsigned long row;
      unsigned long column;

  public:
      Source_Point(const Source_File &source_file, unsigned long row, unsigned long column) :
        source_file(&source_file), column(column), row(row) {}

      Source_Point() : source_file(nullptr), row(0), column(0) {}

      const Source_File &get_source_file() const {
        return *source_file;
      }

      unsigned long get_column() const {
        return column;
      }

      unsigned long get_row() const {
        return row;
      }

      const std::string to_string() const {
        return (source_file ? source_file->get_file_path() : "")
               + " (" + std::to_string(row) + ":" + std::to_string(column) + ")";
      }
  };
}