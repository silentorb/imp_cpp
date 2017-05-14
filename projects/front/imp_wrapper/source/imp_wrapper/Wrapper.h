#pragma once

#include <underworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon.h>
#include <underworld/schema/Profession_Library.h>
#include <overworld/schema/Profession_Library.h>
#include <imp_summoning/Lookup.h>
#include <imp_mirror/Element_Map.h>
#include <underworld/Source_Point.h>

namespace imp_wrapper {

  class Wrapper {
      underworld::Dungeon underworld_root;
      overworld::Dungeon overworld_root;
      underworld::Profession_Library underworld_profession_library;
      imp_summoning::Lookup lookup;
      overworld::Profession_Library overworld_profession_library;
      imp_mirror::Element_Map element_map;
      std::vector<std::unique_ptr<underworld::Source_File>> source_files;

  public:
      Wrapper() :
        underworld_root(""),
        lookup(underworld_profession_library),
        overworld_root(underworld_root) {}

      void load_file(const std::string &path);
      void mirror();
      void render(const std::string &output_path);
  };
}