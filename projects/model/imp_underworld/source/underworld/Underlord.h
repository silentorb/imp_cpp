#pragma once

#include <string>
#include <underworld/schema/Dungeon.h>

namespace underworld {

  class Underlord {
      Dungeon root;

  public:
      Underlord() : root("", nullptr) {}

      Dungeon &get_root(){
        return root;
      }
  };
}