#include "professions.h"

namespace overworld {

  static const std::string primitive_names[] = {
    "bool",
    "double",
    "float",
    "int",
    "string",
    "void"
  };

  const std::string Primitive::get_name() const {
    return primitive_names[(int) primitive_type];
  }

}