#pragma once

#include <overworld/schema/professions/Profession_Library.h>
#include <imp_summoning/Zookeeper.h>

namespace cpp_stl {

  class Standard_Library {
      std::unique_ptr<overworld::Dungeon> overworld_dungeon;

      source_mapping::Source_File standard_library_file;
      overworld::External_File vector_file;
      overworld::External_File string_file;
      overworld::External_File memory_file;

      overworld::Dungeon *vector_type = nullptr;
      overworld::Dungeon *unique_pointer = nullptr;
      overworld::Dungeon *string_type = nullptr;

      void initialize_vector(overworld::Profession_Library &profession_library, overworld::Graph &graph);

  public:
      Standard_Library();

      void
      initialize(overworld::Scope &parent, overworld::Profession_Library &profession_library, overworld::Graph &graph);

      void initialize_underworld(imp_summoning::Zookeeper &zookeeper);

      const source_mapping::Source_File &get_source_file() const {
        return standard_library_file;
      }

      overworld::Dungeon &get_unique_pointer() const {
        return *unique_pointer;
      }

      overworld::Dungeon &get_string_type() const {
        return *string_type;
      }

      overworld::Dungeon &get_vector() const {
        return *vector_type;
      }

      const std::unique_ptr<overworld::Dungeon> &get_overworld_dungeon() const {
        return overworld_dungeon;
      }
  };
}