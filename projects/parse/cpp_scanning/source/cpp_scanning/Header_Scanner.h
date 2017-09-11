#pragma once

#include <string>
#include <vector>
#include <source_mapping/Source_Point.h>
#include <runic_cpp/Cpp_Lexicon.h>
#include "Dungeon_Map.h"

namespace cpp_scanning {

  class Header_Scanner {
      Dungeon_Map dungeons;
      std::vector<std::unique_ptr<source_mapping::Source_File>> source_files;
      underworld::Dungeon &root_dungeon;

  public:
      Header_Scanner(underworld::Dungeon &root_dungeon) : root_dungeon(root_dungeon) {}

      void load_header(const std::string &path, runic::Lexer_Bundle<runic_cpp::Symbols> lexer_bundle);
  };
}