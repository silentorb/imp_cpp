#pragma once

#include <overworld/schema/Dungeon.h>
#include <vector>
#include "Profession_File_Map.h"

namespace overworld {
  namespace exploring {

    void gather_header_dependencies(std::vector<File *> &files, const Dungeon &dungeon, Profession_File_Map &file_map);
  }
}