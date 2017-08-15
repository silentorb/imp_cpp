#pragma once

#include <overworld/schema/Dungeon.h>

namespace solving {

  class Dungeon_Cloner {
      overworld::Dungeon &original;

  public:
      Dungeon_Cloner(overworld::Dungeon &original) : original(original) {}

      overworld::Dungeon &clone();
  };
}