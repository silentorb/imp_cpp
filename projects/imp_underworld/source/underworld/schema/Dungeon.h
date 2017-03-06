#pragma once

#include "Scope.h"

namespace underworld {

  using Dungeons = std::map<std::string, std::unique_ptr<Dungeon>>;

  class Dungeon : public Scope {
      const std::string name;
      Dungeons dungeons;

  protected:
      void throw_already_exists(const std::string &member_name) const override;

  public:
      Dungeon(const std::string &name);
      Dungeon &get_or_create_dungeon(const std::string &dungeon_name);
      Dungeon &get_dungeon(const std::string &dungeon_name);

      Dungeons &get_dungeons() {
        return dungeons;
      }

      const Dungeons &get_dungeons() const{
        return dungeons;
      }

      const std::string &get_name() const {
        return name;
      }
  };
}