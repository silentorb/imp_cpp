#pragma once

#include <unordered_map>
#include "professions.h"

namespace overworld {

  class Profession_Library {
      static Primitive primitives[Primitive_Type_Count];
//      std::map<Minion *, Dungeon_Pointer> anonymous_dungeons;

      std::unordered_map<Profession *, std::unique_ptr<Reference>> references;

  public:
      Profession_Library();

      static Unknown &get_unknown();
      static Void &get_void();
      static Unknown &get_not_found();

      static Primitive &get_primitive(Primitive_Type type);

      Reference & get_reference(Profession & profession);
//      void add_anonymous_dungeon(Minion &minion, Dungeon_Pointer &dungeon);
//      Dungeon &create_anonymous_dungeon(Minion &minion);
  };
}