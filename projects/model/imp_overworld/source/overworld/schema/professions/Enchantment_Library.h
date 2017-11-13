#pragma once

#include <overworld/schema/Dungeon.h>
#include "Profession.h"
#include <memory>
#include <unordered_map>

namespace overworld {

  class Enchantment_Library {
      std::unordered_map<std::string, Dungeon *> global_enchantments;

  public:
      Enchantment_Library();

      Dungeon *find_enchantment(const std::string &name);
      static Dungeon &get_external();
      static Dungeon &get_external_name();
      static Dungeon &get_static();
      static Dungeon &get_public();
      static Dungeon &get_private();
      static Dungeon &get_protected();
      static Dungeon &get_value();
      static Dungeon &get_assignment();

  };
}