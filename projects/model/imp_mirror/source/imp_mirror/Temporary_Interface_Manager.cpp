#include "Temporary_Interface_Manager.h"
#include <overworld/schema/Dungeon.h>

using namespace overworld;

namespace imp_mirror {

  Temporary_Interface_Manager::Temporary_Interface_Manager() {}

  overworld::Dungeon &Temporary_Interface_Manager::create_interface(overworld::Minion &minion) {
    auto dungeon = new Dungeon("");
    interfaces.insert(std::make_pair(&minion, std::unique_ptr<Dungeon>(dungeon)));
    return *dungeon;
  }

  overworld::Dungeon &Temporary_Interface_Manager::get_or_create_interface(overworld::Minion &minion) {
    if (interfaces.count(&minion))
      return *interfaces.at(&minion);

    return create_interface(minion);
  }

  Temporary_Interface_Manager::~Temporary_Interface_Manager() {

  }

}