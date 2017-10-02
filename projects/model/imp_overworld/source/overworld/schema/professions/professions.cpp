#include "professions.h"

namespace overworld {

  static const std::string primitive_names[] = {
    "Bool",
    "Double",
    "Float",
    "Int",
    "String",
    "Void"
  };

  const std::string Primitive::get_name() const {
    return primitive_names[(int) primitive_type];
  }

}