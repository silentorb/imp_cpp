#pragma once

#include <overworld/schema/professions/Profession.h>

namespace overworld {

  class Dungeon_Interface : public virtual Profession {
  public:
      virtual Dungeon & get_original() = 0;
  };

};