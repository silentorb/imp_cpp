#include "Profession_Library.h"

namespace underworld {
  Primitive Profession_Library::primitives[] =
    {
      Primitive_Type::Bool,
      Primitive_Type::Double,
      Primitive_Type::Float,
      Primitive_Type::Int,
      Primitive_Type::String,
    };

  Profession_Library::Profession_Library() {

  }

  Primitive &Profession_Library::get_primitive(Primitive_Type type) {
    return primitives[static_cast<int>(type)];
  }
}