#pragma once

#include <unordered_map>
#include <overworld/schema/Dungeon_Variant.h>
#include "professions.h"

namespace overworld {

  using Variant_Array = std::vector<Dungeon_Variant_Owner>;

  class Profession_Library {
      static Primitive primitives[Primitive_Type_Count];
      std::unordered_map<Profession *, std::unique_ptr<Reference>> references;
      std::unordered_map<Dungeon *, Variant_Array> dungeon_variants;

  public:
      Profession_Library();

      static Unknown &get_unknown();
      static Void &get_void();
//      static Unknown &get_not_found();

      static Primitive &get_primitive(Primitive_Type type);

      Reference &get_reference(Profession &profession);

      Dungeon_Variant &get_or_create_variants(Dungeon &dungeon, std::vector<Profession *> &professions);
  };
}