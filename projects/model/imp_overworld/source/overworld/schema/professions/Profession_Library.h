#pragma once

#include <unordered_map>
#include <overworld/schema/Dungeon_Variant.h>
#include <overworld/schema/Function_Variant.h>
#include <overworld/schema/Enchantment.h>
#include "professions.h"
#include "Enchantment_Library.h"

namespace overworld {

  using Dungeon_Variant_Array = std::vector<Profession_Reference>;
  using Function_Variant_Array = std::vector<Function_Variant_Owner>;
  using Function_Variant_Map = std::unordered_map<Function_Interface *, Function_Variant_Array>;
  using Dungeon_Variant_Tuple = std::tuple<Profession_Reference, Dungeon_Variant &>;

  class Profession_Library {
//      std::unordered_map<Profession *, std::unique_ptr<Reference>> references;
//      std::unordered_map<Profession *, std::unique_ptr<Pointer>> pointers;
      std::unordered_map<Basic_Dungeon *, Dungeon_Variant_Array> dungeon_variants;
      Function_Variant_Map function_variants;
      Graph &graph;
      Enchantment_Library enchantment_library;
//      std::vector<Profession_Owner> profession_store;

  public:
      explicit Profession_Library(Graph &graph);

      static Profession_Reference &get_unknown();
      static Profession_Reference &get_void();
      static Primitive &get_string();
      static File &get_string_file();

      static Profession_Reference &get_primitive(Primitive_Type type);

//      Profession_Reference &get_reference(Profession &profession);
//      Pointer &get_pointer(Profession &profession);

      void assign(Node &node, overworld::Profession_Reference &profession, Profession_Setter &setter);

      Function_Variant_Array &get_function_variant_array(Function_Interface &function);
      Dungeon_Variant_Array &get_dungeon_variant_array(Basic_Dungeon &dungeon);

      static Function_Variant &create_function_variant(Function_Variant_Array &variant_array,
                                                       Function_Interface &function,
                                                       Basic_Dungeon &dungeon,
                                                       std::vector<Profession_Reference> &professions);
      static Function_Variant *get_function_variant(Function_Variant_Array &variant_array, Function_Interface &function,
                                                    std::vector<Profession_Reference> &professions);

      static Dungeon_Variant_Tuple create_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                     Basic_Dungeon &dungeon,
                                                     std::vector<Profession_Reference> &professions);

      static Profession_Reference *get_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                  std::vector<Profession_Reference> &professions);

      Profession_Reference get_or_create_dungeon_variant(Dungeon &dungeon, std::vector<Profession_Reference> &professions,
                                                     overworld::Graph &graph);

      Dungeon_Variant &create_dungeon_variant(overworld::Dungeon_Variant_Array &variant_array,
                                              overworld::Dungeon &dungeon,
                                              Node &starting_node, overworld::Profession_Reference &profession);
//      Dungeon_Variant &resolve_with_existing_template_function(Node &node, Profession &profession);

//      void store_profession(Profession_Owner profession);

      Enchantment_Library &get_enchantment_library() {
        return enchantment_library;
      }

      const Enchantment_Library &get_enchantment_library() const {
        return enchantment_library;
      }
  };
}