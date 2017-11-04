#include <overworld/schema/Dungeon_Reference.h>
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

  Profession_Reference &Profession::get_base(Profession_Reference &self) {
    return <#initializer#>;
  }

  const Profession &Profession::get_base() const {
    return <#initializer#>;
  }
}