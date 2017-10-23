#include <overworld/schema/Dungeon_Variant.h>
#include <overworld/schema/Dungeon_Reference.h>
#include "Connection.h"
#include "Node.h"

namespace overworld {

  Profession_Reference Connection::get_profession(Node &node) {
    return node.get_profession();
  }

  Profession_Reference Variant_To_Argument::get_profession(Node &node) {
    auto &first = get_first();
    auto &second = get_second();

    if (&node == &get_first()) {
      auto &compound_profession = first.get_profession();
      auto &dungeon = compound_profession->get_dungeon_interface();
      auto variant = static_cast<Dungeon_Variant *>(&dungeon);
      auto &arguments = variant->get_arguments();
      return arguments[parameter_index]->get_node().get_profession();
    }
    else {
      auto &compound_profession = first.get_profession();
      auto &dungeon = compound_profession->get_dungeon_interface();
      auto variant = static_cast<Dungeon_Variant *>(&dungeon);
      std::vector<overworld::Profession_Reference> professions;
      professions.push_back(second.get_profession());
      auto new_variant = new Dungeon_Variant(dungeon.get_original(), professions);
      auto dungeon_reference = new Dungeon_Reference(Dungeon_Interface_Owner(new_variant));
      return Profession_Reference(dungeon_reference);
    }
  }
}