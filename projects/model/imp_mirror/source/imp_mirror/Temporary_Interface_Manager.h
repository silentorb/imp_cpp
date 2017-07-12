#pragma once

#include <overworld/schema/Scope.h>
#include <unordered_map>

namespace imp_mirror {

  class Temporary_Interface_Manager {
      std::unordered_map<overworld::Minion *, std::unique_ptr<overworld::Dungeon>> interfaces;

  public:
      Temporary_Interface_Manager();
      ~Temporary_Interface_Manager();
      overworld::Dungeon &create_interface(overworld::Minion &minion);
      overworld::Dungeon &get_or_create_interface(overworld::Minion &minion);
  };
}