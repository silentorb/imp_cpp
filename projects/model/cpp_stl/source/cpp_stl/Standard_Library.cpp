#include <overworld/schema/Function.h>
#include "Standard_Library.h"

using namespace overworld;

namespace cpp_stl {

  Standard_Library::Standard_Library() :
    standard_library_file("[standard_library]"),
    vector_file("vector"),
    string_file("string"),
    memory_file("memory") {
  }

  void Standard_Library::initialize_vector(overworld::Profession_Library &profession_library, overworld::Graph &graph) {
//    source_mapping::Source_Point source_point(standard_library_file, 0, 0);
//    auto &vector = add_dungeon(std::unique_ptr<Dungeon>(new Cpp_Dungeon("vector", "std::vector", *this)));
//    vector_type = &vector;
//    vector.set_file(vector_file);
//    vector.set_default_ownership(Ownership::value);
//    auto &generic_parameter = vector.add_generic_parameter();
//    auto &void_type = profession_library.get_void();
//    auto &push_back = vector.create_function("push_back", void_type, source_point);
//    auto parameter = std::unique_ptr<Owning_Parameter>(
//      new Owning_Parameter("item", generic_parameter, source_point, push_back)
//    );
//    push_back.add_parameter(std::move(parameter));
  }

  void Standard_Library::initialize_underworld(imp_summoning::Zookeeper &zookeeper) {
    zookeeper.load_file("resources/stl/vector.imp");
  }

  void Standard_Library::initialize_overworld(overworld::Scope &parent,
                                              overworld::Profession_Library &profession_library,
                                              overworld::Graph &graph) {
    overworld_dungeon = new overworld::Dungeon("stl", parent);
    parent.add_dungeon(std::unique_ptr<overworld::Dungeon>(overworld_dungeon));
    overworld_dungeon->set_is_external(true);

    source_mapping::Source_Point source_point(standard_library_file, 0, 0);

//    initialize_vector(profession_library, graph);
    // Currently only used to track include file references.
    unique_pointer = &overworld_dungeon->add_dungeon(std::unique_ptr<Dungeon>(
      new Cpp_Dungeon("unique_ptr", "std::unique_ptr", *overworld_dungeon)
    ));
    unique_pointer->set_file(memory_file);

    string_type = &overworld_dungeon->add_dungeon(std::unique_ptr<Dungeon>(
      new Cpp_Dungeon("string", "std::string", *overworld_dungeon)
    ));
    string_type->set_file(string_file);
  }
}
