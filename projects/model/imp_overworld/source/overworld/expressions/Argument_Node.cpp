#include <overworld/schema/professions/Profession_Library.h>
#include "Argument_Node.h"

namespace overworld {
/*
  void Generic_Argument_Element::set_profession(Profession &value, Profession_Setter &setter) {
    if (profession == &value)
      return;

    profession = &value;

    auto &dungeon = dynamic_cast<Dungeon_Interface &>(member.get_profession());
    auto &variant_array = profession_library.get_dungeon_variant_array(dungeon);
    auto &container_profession = profession_library.create_dungeon_variant(
      variant_array, dungeon.get_original(), node, value);
    setter.set_profession(member.get_node(), container_profession);
  }
  */
}