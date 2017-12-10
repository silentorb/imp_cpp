#pragma once

#include <overworld/schema/Temporary_Interface.h>
#include <vector>
#include <memory>

namespace imp_mirror {

  class Temporary_Interface_Manager {
//      std::unordered_map<overworld::Minion *, std::unique_ptr<overworld::Dungeon>> interfaces;
      std::vector<std::unique_ptr<overworld::Temporary_Interface>> interfaces;

  public:
      Temporary_Interface_Manager();
      ~Temporary_Interface_Manager();
//      overworld::Dungeon &create_interface(overworld::Minion &minion);
//      overworld::Dungeon &get_or_create_interface(overworld::Minion &minion);

      void add_interface(std::unique_ptr<overworld::Temporary_Interface> temporary_interface) {
        interfaces.push_back(std::move(temporary_interface));
      }
  };
}