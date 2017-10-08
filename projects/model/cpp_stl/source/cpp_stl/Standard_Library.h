#pragma once

#include <overworld/schema/professions/Profession_Library.h>
#include <imp_summoning/Zookeeper.h>

namespace cpp_stl {

  class Standard_Library {
      overworld::Dungeon overworld_dungeon;
      underworld::Dungeon underworld_dungeon;

      overworld::File memory_file;

      overworld::Dungeon *unique_pointer = nullptr;
      overworld::Dungeon *string_type = nullptr;

  public:
      Standard_Library();

      void
      initialize_overworld(overworld::Profession_Library &profession_library,
                           overworld::Graph &graph);

      void initialize_underworld(imp_summoning::Zookeeper &zookeeper);

      overworld::Dungeon &get_unique_pointer() const {
        return *unique_pointer;
      }

      overworld::Dungeon &get_string_type() const {
        return *string_type;
      }

      overworld::Dungeon &get_overworld_dungeon() {
        return overworld_dungeon;
      }

      underworld::Dungeon &get_underworld_dungeon() {
        return underworld_dungeon;
      }

      const overworld::Dungeon &get_overworld_dungeon() const {
        return overworld_dungeon;
      }
  };
}