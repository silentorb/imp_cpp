#pragma once

#include <string>
#include <memory>
#include <vector>

namespace source_mapping {

  class Source_File {
      const std::string file_path;

  public:
      Source_File(const std::string &file_path) : file_path(file_path) {}

      const std::string &get_file_path() const {
        return file_path;
      }

      const std::string get_short_file_path() const {
        return file_path.substr(file_path.find_last_of("/") + 1);
      }
  };

  using Measurement = unsigned long;
  
  class Source_Point {
      const Source_File *source_file;
      Measurement row;
      Measurement column;

  public:
      Source_Point(const Source_File &source_file, Measurement row, Measurement column) :
        source_file(&source_file), column(column), row(row) {
        int k = 0;
      }

      Source_Point() : source_file(nullptr), row(0), column(0) {}

      const Source_File *get_source_file() const {
        return source_file;
      }

      Measurement get_column() const {
        return column;
      }

      Measurement get_row() const {
        return row;
      }

      const std::string to_string() const {
        return (source_file ? source_file->get_short_file_path() : "")
               + " (" + std::to_string(row) + ":" + std::to_string(column) + ")";
      }
  };

  class Source_Range {
      Source_Point start;
      Source_Point end;

  public:
      Source_Range() {}

      Source_Range(const Source_Point &start, const Source_Point &end) : start(start), end(end) {}
      Source_Range(const Source_Point &start) : start(start), end(start) {}

      const Source_Point &get_start() const {
        return start;
      }

      const Source_Point &get_end() const {
        return end;
      }

      const Source_File *get_source_file() const {
        return start.get_source_file();
      }

  };
  using Source_File_Storage = std::vector<std::unique_ptr<source_mapping::Source_File>>;
}