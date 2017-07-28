#pragma once

#include <overworld/schema/professions/Profession_Library.h>

namespace cpp_stl {

  class Standard_Library : public overworld::Dungeon {
      underworld::Source_File standard_library_file;
      overworld::File vector_file;
      overworld::File string_file;
      overworld::File memory_file;

      overworld::Dungeon *vector_type = nullptr;
      overworld::Dungeon *unique_pointer = nullptr;
      overworld::Dungeon *string_type = nullptr;

  public:
      Standard_Library(overworld::Scope &parent);

      void initialize(overworld::Profession_Library &profession_library);

      const underworld::Source_File &get_source_file() const {
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

  };
}