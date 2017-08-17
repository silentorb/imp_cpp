#include "Profession_Library.h"
#include "cloning.h"

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

  Dungeon_Variant &Profession_Library::get_or_create_variants(Dungeon &dungeon,
                                                              std::vector<Profession *> &professions) {
    Variant_Array *variants;

    if (!dungeon_variants.count(&dungeon)) {
      variants = &dungeon_variants.emplace(&dungeon, Variant_Array()).first->second;
    }
    else {
      variants = &dungeon_variants.at(&dungeon);
    }

    for (auto &variant : *variants) {
      if (professions_match(variant->get_professions(), professions))
        return *variant;
    }

    auto variant = new Dungeon_Variant(dungeon, professions);
    variants->push_back(Dungeon_Variant_Owner(variant));
    clone_dungeon(*variant);
  }
}