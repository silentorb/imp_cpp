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
      auto &new_member = replacement.get_member(member->get_name());
//      old_member_node.disconnect(old_dungeon_node);
      old_member_node.replace_with(new_member.get_node());

      auto temporary_member = dynamic_cast<Temporary_Minion *>(member.get());
      if (new_member.get_member_type() == Member_Type::function) {
        auto & member_function = static_cast<Member_Function &>(new_member);
        auto &new_function = member_function.get_function();
        auto &signature = temporary_member->get_or_create_signature();
        auto &parameters = signature.get_parameters();
        auto &new_parameters = new_function.get_parameters();
        if (parameters.size() != new_parameters.size())
          throw std::runtime_error("New and old parameter lengths does not match.");

        for (auto i = 0; i < parameters.size(); ++i) {
          parameters[i]->get_node().replace_with(new_parameters[i]->get_node());
        }

        if (signature.get_return_type()) {
          throw std::runtime_error("Not implemented.");
        }
      }

      temporary_member->replace_with(new_member);
    }

    for (auto &member : old.get_functions()) {
//      auto old_member_node = member->get_node();
//      auto &new_member = replacement.get_minion(member->get_name());
//      old_member_node.disconnect(old_dungeon_node);
//      old_member_node.replace_with(new_member.get_node());
    }
  }

//  void Parameter::set_profession(Profession &value, Profession_Setter &setter) {
//    if (temporary_interface) {
//      replace_dungeon_node(*temporary_interface, *dynamic_cast<Dungeon *>( &value));
//      temporary_interface.reset();
//    }
//    setter.set_profession(get_node(), value);
//  }

}