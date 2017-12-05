#include <overworld/schema/Function.h>
#include <overworld/expressions/Literal.h>
#include "Standard_Library.h"

using namespace overworld;

namespace cpp_stl {

  Standard_Library::Standard_Library() :
    overworld_dungeon("stl"),
    underworld_dungeon("stl", nullptr),
    memory_file("memory", true) {
    initialize_enchantments();
  }

  overworld::Dungeon &create_dungeon(const std::string &name, overworld::Dungeon &parent) {
    auto new_dungeon = new overworld::Dungeon(name, parent.get_scope());
    parent.add_dungeon(overworld::Dungeon_Owner(new_dungeon));
    return *new_dungeon;
  }

  void Standard_Library::initialize_enchantments() {
    auto &enchantments_dungeon = create_dungeon("enchantments", overworld_dungeon);
    auto &functional = create_dungeon("functional", enchantments_dungeon);
    enchantments.input_stream = &create_dungeon("input_stream", enchantments_dungeon);
    enchantments.functional.map = &create_dungeon("map", functional);
  }

  void Standard_Library::initialize_underworld(imp_summoning::Zookeeper &zookeeper) {
//    zookeeper.load_file("resources/stl/enchantments.imp", underworld_dungeon);
    zookeeper.load_file("resources/stl/Array.imp", underworld_dungeon);
    zookeeper.load_file("resources/stl/iostream.imp", underworld_dungeon);
  }

  void set_external_name(Dungeon &dungeon, const std::string name) {
    auto &external = Enchantment_Library::get_external_name();
    auto complex_enchantment = new overworld::Enchantment(external);
    dungeon.get_enchantments().add_enchantment(overworld::Enchantment_Owner(complex_enchantment));
    complex_enchantment->add_argument(
      Expression_Owner(new Literal_String(name, Parent(), source_mapping::Source_Range())));
  }

  void Standard_Library::initialize_overworld(overworld::Profession_Library &profession_library) {
    set_external_name(overworld_dungeon, "std");
    overworld_dungeon.get_enchantments().add_enchantment(profession_library.get_enchantment_library().get_external());

//    source_mapping::Source_Range source_point(standard_library_file, 0, 0);

    unique_pointer = &overworld_dungeon.add_dungeon(std::unique_ptr<Dungeon>(
      new Dungeon("unique_ptr", overworld_dungeon.get_scope())
    ));
    unique_pointer->set_file(memory_file);
    set_external_name(*unique_pointer, "std::unique_ptr");

    string_type = &overworld_dungeon.add_dungeon(std::unique_ptr<Dungeon>(
      new Dungeon("String", overworld_dungeon.get_scope())
    ));
    string_type->set_file(Profession_Library::get_string_file());
    set_external_name(*string_type, "std::string");
    string_type->get_enchantments().add_enchantment(Enchantment_Library::get_value());
    string_type->set_default_storage(overworld::Storage_Type::value);
  }
}
