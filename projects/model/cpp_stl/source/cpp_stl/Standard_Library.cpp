#include <overworld/schema/Function.h>
#include "Standard_Library.h"

namespace cpp_stl {

  Standard_Library::Standard_Library(overworld::Scope &parent, overworld::Profession_Library &profession_library,
                                     overworld::Graph &graph) :
    Dungeon("stl", parent), file("standard_library") {
    set_is_external(true);

    underworld::Source_Point source_point(file, 0, 0);
    auto &vector = create_dungeon("vector");
    auto &void_type = profession_library.get_void();
    auto &push_back = vector.create_function("push_back", void_type, source_point);
    push_back.create_parameter("item", profession_library.get_unknown());
  }
}
