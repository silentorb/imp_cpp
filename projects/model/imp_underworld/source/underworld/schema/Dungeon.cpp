#include "Dungeon.h"

using namespace std;

namespace underworld {

  Dungeon::Dungeon(const std::string &name) :
    name(name) {}

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
//
//  Dungeon &Dungeon::get_dungeon(const std::string &dungeon_name) {
//    auto search = dungeons.find(dungeon_name);
//    if (search != dungeons.end()) {
//      return *(*search).second;
//    }
//
//    if (name == "")
//      throw runtime_error("There is no dungeon named " + dungeon_name + ".");
//    else
//      throw runtime_error("Dungeon " + name + " does not have a child dungeon named " + dungeon_name + ".");
//  }

  void Dungeon::throw_already_exists(const std::string &member_name) const {
    throw std::runtime_error("Dungeon " + name + " already has a member named " + member_name + ".");
  }

  Dungeon::~Dungeon() {

  }

}