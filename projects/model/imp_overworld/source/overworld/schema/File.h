#pragma once

#include <string>

namespace overworld {

  class File {
      const std::string folder;
      const std::string filename;

  public:
      File(const std::string &folder, const std::string &filename) :
        folder(folder), filename(filename) {}

      File(const std::string &filename) :
        folder(""), filename(filename) {}

      const std::string get_folder() const {
        return folder;
      }

      const std::string get_filename() const {
        return filename;
      }
  };
}