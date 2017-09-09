#pragma once

#include <string>
#include <vector>
#include <source_mapping/Source_Point.h>
#include "Dungeon_Map.h"

namespace cpp_scanning {

  class Header_Scanner {
      Dungeon_Map dungeons;
      std::vector<std::unique_ptr<source_mapping::Source_File>> source_files;


  public:
      void load_header(const std::string &path);
  };
}