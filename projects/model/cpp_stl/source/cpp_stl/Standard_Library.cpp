#include <overworld/schema/Function.h>
#include <overworld/schema/Enchantment_With_Arguments.h>
#include <overworld/expressions/Literal.h>
#include "Standard_Library.h"

using namespace overworld;

namespace cpp_stl {

  Standard_Library::Standard_Library() :
    standard_library_file("[standard_library]"),
    memory_file("memory") {
  }

  void Standard_Library::initialize_underworld(imp_summoning::Zookeeper &zookeeper) {
    zookeeper.load_file("resources/stl/vector.imp");
//    zookeeper.load_file("resources/stl/iostream.imp");
  }

  void set_external_name(Dungeon &dungeon, const std::string name) {
    auto &external = Enchantment_Library::get_external_name();
    auto complex_enchantment = new overworld::Enchantment_With_Arguments(external);
    dungeon.get_enchantments().add_enchantment(overworld::Enchantment_Owner(complex_enchantment));
    complex_enchantment->add_argument(
      Expression_Owner(new Literal_String(name, nullptr, source_mapping::Source_Point(), nullptr)));
  }

  void Standard_Library::initialize_overworld(overworld::Scope &parent,
                                              overworld::Profession_Library &profession_library,
                                              overworld::Graph &graph) {
    overworld_dungeon = new overworld::Dungeon("stl", parent);
    parent.add_dungeon(std::unique_ptr<overworld::Dungeon>(overworld_dungeon));
    set_external_name(*overworld_dungeon, "std");
    overworld_dungeon->get_enchantments().add_enchantment(profession_library.get_enchantment_library().get_external());

    source_mapping::Source_Point source_point(standard_library_file, 0, 0);

    unique_pointer = &overworld_dungeon->add_dungeon(std::unique_ptr<Dungeon>(
      new Dungeon("unique_ptr", *overworld_dungeon)
    ));
    unique_pointer->set_file(memory_file);
    set_external_name(*unique_pointer, "std::unique_ptr");

    string_type = &overworld_dungeon->add_dungeon(std::unique_ptr<Dungeon>(
      new Dungeon("string", *overworld_dungeon)
    ));
    string_type->set_file(Profession_Library::get_string_file());
    set_external_name(*string_type, "std::string");
  }
}
