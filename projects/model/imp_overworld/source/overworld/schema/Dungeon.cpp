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


}