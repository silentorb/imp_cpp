#include <overworld/schema/Dungeon_Reference.h>
#include <overworld/schema/Dungeon.h>
#include "Profession.h"
#include "professions.h"

namespace overworld {

  bool can_cast(Profession &target, Profession &source) {
    if (&target.get_base() == &source.get_base())
      return true;

    auto contracts_or_null = source.get_contracts();
    if (contracts_or_null) {
      auto contracts = *contracts_or_null;
      for (auto contract:  contracts) {
        if (can_cast(target, *contract))
          return true;
      }
    }

    return false;
  }

  Dungeon &Profession::get_dungeon_interface() {
    auto dungeon_reference = static_cast<Dungeon_Reference *>(this);
    return dungeon_reference->get_dungeon_interface();
  }

  const Dungeon &Profession::get_dungeon_interface() const {
    auto dungeon_reference = static_cast<const Dungeon_Reference *>(this);
    return dungeon_reference->get_dungeon_interface();
  }

  Dungeon *Profession::as_variant() {
    return get_type() == Profession_Type::dungeon
           ? &get_dungeon_interface()
           : nullptr;
  }

//  Ownership get_profession_default_ownership(Profession &profession) {
//    switch (profession.get_type()) {
//
//      case Profession_Type::dungeon:
//        return profession.get_dungeon_interface().get_ownership();
//
//      case Profession_Type ::primitive:
//        return Ownership::copy;
//
//      case Profession_Type ::reference:
//        return static_cast<Reference&>(profession).get_ownership();
//
//      default:
//        return Ownership::unknown;
//    }
//  }
}