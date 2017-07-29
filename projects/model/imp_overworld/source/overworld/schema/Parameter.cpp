#include "Parameter.h"
#include "Temporary_Minion.h"

namespace overworld {

  Parameter::~Parameter() {

  }

  overworld::Dungeon &Parameter::get_or_create_interface() {
    if (temporary_interface)
      return *temporary_interface;

    auto dungeon = new Dungeon("?" + get_name() + "?");
    temporary_interface = std::unique_ptr<Dungeon>(dungeon);
    return *dungeon;
  }

  void replace_dungeon_node(Dungeon &old, Dungeon &replacement) {
    auto &old_dungeon_node = old.get_node();
    auto &new_dungeon_node = replacement.get_node();

    for (auto &member : old.get_minions()) {
      auto &old_member_node = member->get_node();
      auto &new_member = replacement.get_minion(member->get_name());
//      old_member_node.disconnect(old_dungeon_node);
      old_member_node.replace_with(new_member.get_node());

      auto temporary_member = dynamic_cast<Temporary_Minion *>(member.get());
      temporary_member->replace_with(new_member);
    }

    for (auto &member : old.get_functions()) {
//      auto old_member_node = member->get_node();
//      auto &new_member = replacement.get_minion(member->get_name());
//      old_member_node.disconnect(old_dungeon_node);
//      old_member_node.replace_with(new_member.get_node());
    }
  }

  void Parameter::set_profession(Profession &value) {
    if (temporary_interface) {
      replace_dungeon_node(*temporary_interface, *dynamic_cast<Dungeon *>( &value));
      temporary_interface.reset();
    }
    Minion::set_profession(value);
  }

}