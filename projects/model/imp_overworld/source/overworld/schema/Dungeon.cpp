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

  Dungeon &Dungeon::create_dungeon(const std::string &name) {
    auto dungeon = new Dungeon(name, *this);
    auto pointer = unique_ptr<Dungeon>(dungeon);
    add_dungeon(pointer);
    members[name] = dungeon;
    if (_is_external)
      dungeon->set_is_external(true);

    return *dungeon;
  }

//  Dungeon &Dungeon::create_dungeon(underworld::Dungeon &input_dungeon) {
//    auto dungeon = new Dungeon(input_dungeon);
//    dungeons.push_back(unique_ptr<Dungeon>(dungeon));
//    return *dungeon;
//  }

//  void Dungeon::add_dungeon(Dungeon_Owner &dungeon) {
//    dungeons.push_back(std::move(dungeon));
//  }
//  Dungeon &Dungeon::get_or_create_dungeon(const std::string &dungeon_name) {
//    if (dungeon_name == "")
//      throw runtime_error("Dungeon name cannot be empty.");
//
//    auto search = dungeons.find(dungeon_name);
//    if (search != dungeons.end()) {
//      return *(*search).second;
//    }
//
//    check_has_member(dungeon_name);
//
//    auto dungeon = new Dungeon(dungeon_name);
//    dungeons[dungeon_name] = unique_ptr<Dungeon>(dungeon);
//    return *dungeon;
//  }

//  Dungeon &Dungeon::get_dungeon(const std::string &dungeon_name) {
//    auto search = dungeons.find(dungeon_name);
//    if (search != dungeons.end()) {
//      return *(*search).second;
//    }
//
//    if (source.get_name() == "")
//      throw runtime_error("There is no dungeon named " + dungeon_name + ".");
//    else
//      throw runtime_error(
//        "Dungeon " + source.get_name() + " does not have a child dungeon named " + dungeon_name + ".");
//  }
//
//  void Dungeon::throw_already_exists(const std::string &member_name) const {
//    throw std::runtime_error("Dungeon " + source.get_name() + " already has a member named " + member_name + ".");
//  }


}