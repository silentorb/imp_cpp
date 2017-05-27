#pragma once

#include <string>

namespace overworld {

  class File {
      std::string path;

  public:
      File(const std::string &path) : path(path){}

      const std::string get_path() const {
        return path;
      }
  };
}