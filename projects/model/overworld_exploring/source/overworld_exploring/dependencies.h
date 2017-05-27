#pragma once

#include <overworld/schema/Dungeon.h>
#include <vector>

namespace overworld {
  namespace exploring {

    void gather_header_dependencies(std::vector<File *> &files, const Dungeon &dungeon);
  }
}