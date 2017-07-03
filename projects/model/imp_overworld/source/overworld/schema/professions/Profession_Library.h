#pragma once

#include "professions.h"

namespace overworld {

  class Profession_Library {
      static Primitive primitives[Primitive_Type_Count];
      std::map<Minion *, Dungeon_Pointer> anonymous_dungeons;

  public:
      Profession_Library();

      static const Unknown &get_unknown();
      static const Unknown &get_not_found();

      static Primitive &get_primitive(Primitive_Type type);

      void add_anonymous_dungeon(Minion & minion, Dungeon_Pointer & dungeon);
      Dungeon & create_anonymous_dungeon(Minion & minion);
  };
}