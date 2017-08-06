#include "Dungeon.h"
#include "Function.h"

using namespace std;

namespace overworld {

  Function &Dungeon::get_or_create_constructor() {
    for (auto &method: functions) {
      if (method->get_name() == get_name())
        return *method;
    }

    return create_function(get_name(), *this, underworld::Source_Point());
//    throw std::runtime_error("Could not find constructor.");
  }

//  Dungeon &Dungeon::create_dungeon(const std::string &name) {
//    auto dungeon = new Dungeon(name, *this);
//    auto pointer = unique_ptr<Dungeon>(dungeon);
//    add_dungeon(std::move(pointer));
//    members[name] = dungeon;
//    if (_is_external)
//      dungeon->set_is_external(true);
//
//    return *dungeon;
//  }

  Dungeon &Dungeon::add_dungeon(std::unique_ptr<Dungeon> dungeon) {
    if (_is_external)
      dungeon->set_is_external(true);

    auto result = dungeon.get();
    Scope::add_dungeon(dungeon);
    return *result;
  }

  Minion &Dungeon::get_minion(const std::string &name) {
    for (auto &variable: minions) {
      if (variable->get_name() == name)
        return *variable;
    }

    if (base_dungeon) {
      return base_dungeon->get_minion(name);
    }

    throw std::runtime_error("Could not find variable named " + name);
  }

  Member &Dungeon::get_member(const std::string &name) {
    if (members.count(name) != 0)
      return *members.at(name);

    if (base_dungeon) {
      return base_dungeon->get_member(name);
    }

    throw std::runtime_error("Could not find member: " + name);
  }


}