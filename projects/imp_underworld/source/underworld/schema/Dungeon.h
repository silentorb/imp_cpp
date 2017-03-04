#pragma once

#include "Scope.h"

namespace underworld {

  class Dungeon : public Scope {
      const std::string name;

      std::map<std::string, std::unique_ptr<Dungeon>> dungeons;

  protected:
      void throw_already_exists(const std::string &member_name) const override;

  public:
      Dungeon(const std::string &name);
      Dungeon &get_or_create_dungeon(const std::string &name);

  };
}