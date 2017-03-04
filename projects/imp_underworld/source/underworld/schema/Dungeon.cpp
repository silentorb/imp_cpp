#include "Dungeon.h"

using namespace std;

namespace underworld {

  Dungeon::Dungeon(const std::string &name) :
    name(name) {}

  Dungeon &Dungeon::get_or_create_dungeon(const std::string &name) {
    auto search = dungeons.find(name);
    if (search != dungeons.end()) {
      return *(*search).second;
    }

    check_has_member(name);

    auto dungeon = new Dungeon(name);
    dungeons[name] = unique_ptr<Dungeon>(dungeon);
    return *dungeon;
  }

  void Dungeon::throw_already_exists(const std::string &member_name) const {
    throw std::runtime_error("Dungeon " + this->name + " already has a member named " + name + ".");
  }

}