#pragma once

#include <underworld/schema/Dungeon.h>

namespace cpp_scanning {

  class Context {
      underworld::Dungeon *dungeon;

  public:
      Context() : dungeon(nullptr) {}

      Context(underworld::Dungeon &dungeon) : dungeon(&dungeon) {}
  };
}