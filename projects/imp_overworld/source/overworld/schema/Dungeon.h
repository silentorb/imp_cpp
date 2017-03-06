#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Scope {
      const underworld::Dungeon &source;
      Dungeons dungeons;

  public:
      Dungeon(const underworld::Dungeon &source);
      Dungeon &add_dungeon(Dungeon_Owner &dungeon);
//      Dungeon &get_dungeon(const std::string &dungeon_name);

      Dungeons &get_dungeons() {
        return dungeons;
      }
  };
}