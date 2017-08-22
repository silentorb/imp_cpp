#include "Profession_Library.h"

namespace overworld {

  File string_header("", "string");
  static Unknown unknown;
  static Void _void;
//  static Unknown not_found;

  Primitive Profession_Library::primitives[] =
    {
      Primitive_Type::Bool,
      Primitive_Type::Double,
      Primitive_Type::Float,
      Primitive_Type::Int,
      {Primitive_Type::String, &string_header},
//      Primitive_Type::Void,
    };

  Profession_Library::Profession_Library() {

  }

  Unknown &Profession_Library::get_unknown() {
    return unknown;
  }

  Void &Profession_Library::get_void() {
    return _void;
  }

//  Unknown &Profession_Library::get_not_found() {
//    return not_found;
//  }

  Primitive &Profession_Library::get_primitive(Primitive_Type type) {
    return primitives[static_cast<int>(type)];
  }

  Reference &Profession_Library::get_reference(Profession &profession) {
    if (references.count(&profession))
      return *references[&profession];

    auto reference = new Reference(profession);
    references[&profession] = std::unique_ptr<Reference>(reference);
    return *reference;
  }

  template<typename A, typename B>
  B &get_or_create_variant_map(std::unordered_map<A *, B> &generic_map, A &item) {
    if (!generic_map.count(&item)) {
      return generic_map.emplace(&item, B()).first->second;
    }
    else {
      return generic_map.at(&item);
    }
  };

  Function_Variant_Array &Profession_Library::get_function_variant_array(Function_Interface &function) {
    return get_or_create_variant_map(function_variants, function);
  }

  Dungeon_Variant_Array &Profession_Library::get_dungeon_variant_array(Dungeon_Interface &dungeon) {
    return get_or_create_variant_map(dungeon_variants, dungeon);
  }


  Function_Variant &Profession_Library::create_function_variant(Function_Variant_Array &variant_array,
                                                                Function_Interface &function,
                                                                Dungeon_Interface &dungeon,
                                                                std::vector<Profession *> &professions) {

    auto variant = new Function_Variant(function.get_original(), dungeon, professions);
    variant_array.push_back(Function_Variant_Owner(variant));
    return *variant;
  }

  template<typename A>
  A *find_variant(std::vector<std::unique_ptr<A>> &variant_array, std::vector<Profession *> &professions) {
    for (auto &variant : variant_array) {
      if (professions_match(variant->get_generic_parameters(), professions))
        return variant.get();
    }

    return nullptr;
  }

  Function_Variant *Profession_Library::get_function_variant(Function_Variant_Array &variant_array,
                                                             Function_Interface &function,
                                                             std::vector<Profession *> &professions) {
    return find_variant(variant_array, professions);
//    for (auto &variant : variant_array) {
//      if (professions_match(variant->get_generic_parameters(), professions))
//        return variant.get();
//    }
//
//    return nullptr;
  }

  Dungeon_Variant &Profession_Library::create_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                              Dungeon_Interface &dungeon,
                                                              std::vector<Profession *> &professions) {
    auto variant = new Dungeon_Variant(dungeon.get_original(), professions);
    variant_array.push_back(Dungeon_Variant_Owner(variant));
    return *variant;
  }

  Dungeon_Variant *Profession_Library::get_dungeon_variant(Dungeon_Variant_Array &variant_array,
                                                           std::vector<Profession *> &professions) {
    return find_variant(variant_array, professions);
//    for (auto &variant : variant_array) {
//      if (professions_match(variant->get_generic_parameters(), professions))
//        return variant.get();
//    }
//
//    return nullptr;
  }
}