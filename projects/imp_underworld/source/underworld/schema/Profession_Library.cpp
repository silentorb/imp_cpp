#include "Profession_Library.h"

namespace underworld {

  Profession_Library::Profession_Library() :
    primitives(
      {
        Primitive_Type::Bool,
        Primitive_Type::Double,
        Primitive_Type::Float,
        Primitive_Type::Int,
        Primitive_Type::String,
      }) {

  }
}