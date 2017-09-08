#pragma once

#include <string>
#include <schema/Class.h>
#include <memory>
#include <vector>

namespace cpp_scanning {

  class Header_Scanner {
      std::vector<std::unique_ptr<Class>> classes;

  public:
      void load_header(const std::string &path);
  };
}