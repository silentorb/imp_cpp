#include "Standard_Library.h"

namespace cpp_stl {

  Standard_Library::Standard_Library(overworld::Scope &parent, overworld::Profession_Library &profession_library) :
    Dungeon("stl", nullptr, &parent), file("standard_library") {

    underworld::Source_Point source_point(file, 0, 0);
    throw std:: runtime_error("Not implemented.");
//    auto &vector = create_dungeon("vector", source_point);
//    auto &void_type = profession_library.get_primitive(overworld::Primitive_Type::Void);
//    vector.create_function("push_back", void_type, source_point);
  }
}
