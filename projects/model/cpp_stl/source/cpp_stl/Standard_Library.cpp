#include <overworld/schema/Function.h>
#include "Standard_Library.h"

using namespace overworld;

namespace cpp_stl {

  Standard_Library::Standard_Library(overworld::Scope &parent) :
    Dungeon("stl", parent),
    standard_library_file("[standard_library]"),
    vector_file("vector"),
    string_file("string"),
    memory_file("memory") {
    set_is_external(true);
  }

  void Standard_Library::initialize(overworld::Profession_Library &profession_library) {
    underworld::Source_Point source_point(standard_library_file, 0, 0);

    auto &vector = add_dungeon(std::unique_ptr<Dungeon>(new Cpp_Dungeon("vector", "std::vector", *this)));
    vector_type = &vector;
    vector.set_file(vector_file);
    vector.set_default_ownership(Ownership::value);
    auto &void_type = profession_library.get_void();
    auto &push_back = vector.create_function("push_back", void_type, source_point);
    push_back.add_parameter(new Owning_Parameter("item", profession_library.get_unknown(), source_point));

    // Currently only used to track include file references.
    unique_pointer = &add_dungeon(std::unique_ptr<Dungeon>(
      new Cpp_Dungeon("unique_ptr", "std::unique_ptr", *this)
    ));
    unique_pointer->set_file(memory_file);

    string_type = &add_dungeon(std::unique_ptr<Dungeon>(
      new Cpp_Dungeon("string", "std::string", *this)
    ));
    string_type->set_file(string_file);

  }
}
