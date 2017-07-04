#pragma once

#include <overworld/schema/professions/Profession_Library.h>

namespace cpp_stl {

  class Standard_Library : public overworld::Dungeon {
      underworld::Source_File file;

  public:
      Standard_Library(overworld::Scope &parent, overworld::Profession_Library &profession_library,
                       overworld::Graph &graph);

      const underworld::Source_File &get_source_file() const {
        return file;
      }
  };
}