#pragma once

#include <overworld/schema/File.h>
#include <overworld/schema/professions/Profession.h>

namespace overworld {

  class Profession_File_Map {
  public:
      virtual File *get_file(const Profession &dungeon) = 0;

  };
}