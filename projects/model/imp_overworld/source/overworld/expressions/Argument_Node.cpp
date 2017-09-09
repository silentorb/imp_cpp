#include <overworld/schema/professions/Profession_Library.h>
#include "Argument_Node.h"

namespace overworld {

  void Argument_Node::set_profession(Profession &value) {
    if (profession == &value)
      return;

    profession = &value;

    auto &dungeon = dynamic_cast<Dungeon_Interface &>(member_container.get_profession());
    auto &variant_array = profession_library.get_dungeon_variant_array(dungeon);
    auto &container_profession = profession_library.create_dungeon_variant(
      variant_array, dungeon.get_original(), *this, value);
    member_container.set_profession(container_profession);
  }
}