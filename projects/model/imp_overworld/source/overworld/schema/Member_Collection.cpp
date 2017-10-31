#include "Member_Collection.h"
#include "Dungeon.h"

namespace overworld {

  bool can_be_overloaded(Member &member) {
    if (member.get_type() == Member_Type::dungeon) {
      auto &dungeon = member.get_dungeon();
      return !dungeon.get_generic_parameters().empty();
    }
    else if (member.get_type() == Member_Type::function) {
      return true;
    }
    else {
      return false;
    }
  }

  void Member_Collection::overload(const std::string &name, Member &member) {
    auto &existing = members.at(name);
    if (!can_be_overloaded(member) || member.get_type() != existing.get_type())
      throw std::runtime_error("Duplicated identifier: " + name + ".");

    overloaded_members[name] = Members();
    auto &array = overloaded_members[name];
    array.push_back(existing);
    array.push_back(member);
    members.erase(name);
  }

  void Member_Collection::add_overload(const std::string &name, Member &member) {
    auto &array = overloaded_members.at(name);
    auto &first = array[0];

    if (!can_be_overloaded(member) || member.get_type() != first.get_type())
      throw std::runtime_error("Duplicated identifier: " + name + ".");

    array.push_back(member);
  }

  void Member_Collection::add_member(const std::string &name, Member member) {
    if (members.count(name)) {
      overload(name, member);
    }
    else if (overloaded_members.count(name)) {
      add_overload(name, member);
    }
    else {
      members[name] = member;
    }
  }

  Member *Member_Collection::get_member_or_null(const std::string &name) {
    return members.count(name) != 0
           ? &members.at(name)
           : nullptr;
  }

  Member &Member_Collection::get_member(const std::string &name) {
    if (members.count(name) != 0)
      return members.at(name);

    throw std::runtime_error("Could not find member: " + name);
  }
}