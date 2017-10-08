#include "Member.h"
#include "Dungeon.h"
#include "Function.h"

namespace overworld {

  Node &get_member_node(Member &member) {
    switch (member.get_type()) {
      case Member_Type::function:
        return member.get_function().get_node();

      case Member_Type::dungeon:
        return member.get_dungeon().get_node();

      case Member_Type::minion:
        return member.get_minion().get_node();

      case Member_Type::profession:
        return member.get_profession().get_node();

      default:
        throw std::runtime_error("Not implemented.");
    }
  }

  const std::string get_member_name(Member &member) {
    switch (member.get_type()) {
      case Member_Type::function:
        return member.get_function().get_name();

      case Member_Type::dungeon:
        return member.get_dungeon().get_name();

      case Member_Type::minion:
        return member.get_minion().get_name();

      case Member_Type::profession:
        return member.get_profession().get_name();

      default:
        throw std::runtime_error("Not implemented.");
    }
  }

  const Node &get_member_node(const Member &member) {
    return get_member_node(const_cast<Member &>(member));
  }

  const std::string get_member_name(const Member &member) {
    return get_member_name(const_cast<Member &>(member));
  }

}