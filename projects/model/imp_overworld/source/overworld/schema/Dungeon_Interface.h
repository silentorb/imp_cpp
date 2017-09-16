#pragma once

#include <overworld/schema/professions/Profession.h>

namespace overworld {

  enum class Dungeon_Type {
      original,
      variant
  };

  class Dungeon_Interface : public virtual Profession {
  public:
      virtual Dungeon &get_original() = 0;
      virtual const Dungeon &get_original() const = 0;
      virtual Dungeon_Type get_dungeon_type() const = 0;
  };

};