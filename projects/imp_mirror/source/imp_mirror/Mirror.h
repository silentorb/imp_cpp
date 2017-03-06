#pragma once

#include <overworld/schema/Dungeon.h>
#include <underworld/schema/Dungeon.h>
#include <overworld/schema/Profession_Library.h>

namespace imp_mirror {

  class Mirror {
      overworld::Profession_Library &profession_library;

      const overworld::Profession &convert_profession(const underworld::Profession &profession);

  public:
      Mirror(overworld::Profession_Library &profession_library) :
        profession_library(profession_library) {}

      void reflect_dungeon(const underworld::Dungeon &input, overworld::Dungeon &output);
  };
}