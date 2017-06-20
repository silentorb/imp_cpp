#pragma once

#include <underworld/schema/Dungeon.h>
#include <underworld/schema/Profession_Library.h>

namespace cpp_stl {

  class Standard_Library : public underworld::Dungeon {
      underworld::Source_File file;

  public:
      Standard_Library(underworld::Scope &parent, underworld:: Profession_Library & profession_library);

      const underworld::Source_File &get_source_file() const {
        return file;
      }
  };
}