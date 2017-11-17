#include <overworld/schema/Dungeon_Reference.h>
#include <overworld/schema/Dungeon_Variant.h>
#include "Profession.h"

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

  Basic_Dungeon &Profession::get_dungeon_interface() {
    auto dungeon_reference = static_cast<Dungeon_Reference *>(this);
    return dungeon_reference->get_dungeon_interface();
  }

  const Basic_Dungeon &Profession::get_dungeon_interface() const {
    auto dungeon_reference = static_cast<const Dungeon_Reference *>(this);
    return dungeon_reference->get_dungeon_interface();
  }

  Dungeon_Variant *Profession::as_variant() {
    if (get_type() == Profession_Type::dungeon) {
      auto &interface = get_dungeon_interface();
      if (interface.get_dungeon_type() == Dungeon_Type::variant) {
        return dynamic_cast<Dungeon_Variant *>(&interface);
      }
    }

    return nullptr;
  }
}