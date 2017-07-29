#include "Profession_Library.h"

namespace overworld {

  File string_header("", "string");
  static Unknown unknown;
  static Void _void;
  static Unknown not_found;

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

  Unknown &Profession_Library::get_not_found() {
    return not_found;
  }

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
//  void Profession_Library::add_anonymous_dungeon(Minion &minion, Dungeon_Pointer &dungeon) {
//    anonymous_dungeons[&minion] = std::move(dungeon);
//  }

//  Dungeon &Profession_Library::create_anonymous_dungeon(Minion &minion) {
//    auto result = new Anonymous_Dungeon();
//    Dungeon_Pointer dungeon(result);
//    add_anonymous_dungeon(minion, dungeon);
//    return *result;
//  }
}