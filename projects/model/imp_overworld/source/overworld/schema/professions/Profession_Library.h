#pragma once

#include <unordered_map>
#include <overworld/schema/Dungeon_Variant.h>
#include <overworld/schema/Function_Variant.h>
#include "professions.h"

namespace overworld {

  using Dungeon_Variant_Array = std::vector<Dungeon_Variant_Owner>;
  using Function_Variant_Array = std::vector<Function_Variant_Owner>;

  class Profession_Library {
      static Primitive primitives[Primitive_Type_Count];
      std::unordered_map<Profession *, std::unique_ptr<Reference>> references;
      std::unordered_map<Dungeon *, Dungeon_Variant_Array> dungeon_variants;
      std::unordered_map<Function_Interface *, Function_Variant_Array> function_variants;

  public:
      Profession_Library();

      static Unknown &get_unknown();
      static Void &get_void();
//      static Unknown &get_not_found();

      static Primitive &get_primitive(Primitive_Type type);

      Reference &get_reference(Profession &profession);

      Dungeon_Variant &get_or_create_dungeon_variant(Dungeon &dungeon, std::vector<Profession *> &professions,
                                                     Graph &graph);

      Function_Variant &get_or_create_function_variant(Function_Interface &function, std::vector<Profession *> &professions,
                                                             Node &starting_node, Profession &new_profession, Graph &graph);
  };
}