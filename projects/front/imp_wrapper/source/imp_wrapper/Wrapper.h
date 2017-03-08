#pragma once

#include <underworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon.h>
#include <underworld/schema/Profession_Library.h>
#include <overworld/schema/Profession_Library.h>

namespace imp_wrapper {

  class Wrapper {
      underworld::Dungeon underworld_root;
      overworld::Dungeon overworld_root;
      underworld::Profession_Library underworld_profession_library;
      overworld::Profession_Library overworld_profession_library;

  public:
      Wrapper() :
        underworld_root(""),
        overworld_root(underworld_root) {}

      void load_file(const std::string &path);
      void mirror();
      void render(const std::string &output_path);
  };
}