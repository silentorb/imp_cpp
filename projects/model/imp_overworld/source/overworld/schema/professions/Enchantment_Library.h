#pragma once

#include <overworld/schema/Enchantment.h>
#include "Profession.h"
#include <memory>
#include <unordered_map>

namespace overworld {

  class Enchantment_Library {
      std::unordered_map<std::string, Enchantment *> global_enchantments;

  public:
      Enchantment_Library();

      Enchantment *find_enchantment(const std::string &name);
      static Simple_Enchantment &get_external();
      static Simple_Enchantment &get_static();
      static Simple_Enchantment &get_public();
      static Simple_Enchantment &get_private();
      static Simple_Enchantment &get_protected();
      static Simple_Enchantment &get_value();
  };
}