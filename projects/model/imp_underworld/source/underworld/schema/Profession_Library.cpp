#include "Profession_Library.h"

namespace underworld {
  Primitive Profession_Library::primitives[] =
    {
      {Primitive_Type::Bool, "bool"},
      {Primitive_Type::Double, "double"},
      {Primitive_Type::Float, "float"},
      {Primitive_Type::Int, "int"},
      {Primitive_Type::String, "string"},
      {Primitive_Type::Void, "void"},
    };

  Profession_Library::Profession_Library() {

  }

  Primitive &Profession_Library::get_primitive(Primitive_Type type) {
    return primitives[static_cast<int>(type)];
  }
}