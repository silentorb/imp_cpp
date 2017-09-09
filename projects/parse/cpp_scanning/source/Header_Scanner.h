#pragma once

#include <string>
#include <schema/Class.h>
#include <memory>
#include <vector>
#include <source_mapping/Source_Point.h>

namespace cpp_scanning {

  class Header_Scanner {
      std::vector<std::unique_ptr<Class>> classes;
      std::vector<std::unique_ptr<source_mapping::Source_File>> source_files;

  public:
      void load_header(const std::string &path);
  };
}