#pragma once

#include <stdexcept>

namespace underworld {

  class Already_Exists_Error : public std::runtime_error {
  public:
      Already_Exists_Error(const std::string &parent, const std::string &name) :
        runtime_error(parent + "already contains a member named \"" + name + "\".") {}

  };
}