#pragma once

#include <overworld/schema/professions/Profession.h>

namespace overworld {

  enum class Dungeon_Type {
      original,
      variant
  };

  class Dungeon_Interface {
  public:
      virtual Dungeon &get_original() = 0;
      virtual const Dungeon &get_original() const = 0;
      virtual Dungeon_Type get_dungeon_type() const = 0;
      virtual Ownership get_ownership() const = 0;
  };

  using Dungeon_Interface_Owner = std::unique_ptr<Dungeon_Interface>;
};