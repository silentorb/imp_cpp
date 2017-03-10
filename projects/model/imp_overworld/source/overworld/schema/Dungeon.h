#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Scope, public virtual Scope_Parent {
      const underworld::Dungeon &source;
      Dungeons dungeons;

  public:
      Dungeon(const underworld::Dungeon &source);
      Dungeon &create_dungeon(underworld::Dungeon &input_dungeon);
//      Dungeon &get_dungeon(const std::string &dungeon_name);

      Dungeons &get_dungeons() {
        return dungeons;
      }

      const Dungeons &get_dungeons() const {
        return dungeons;
      }

      const std::string &get_name() const {
        return source.get_name();
      }

      Scope_Parent_Type get_scope_parent_type() const override {
        return Scope_Parent_Type::dungeon;
      }
  };
}