#include "Parameter.h"

namespace overworld {

  Parameter::~Parameter() {

  }

  void replace_dungeon_node(Dungeon &old, Dungeon &replacement) {
    for(auto & member : old.get_minions()) {

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