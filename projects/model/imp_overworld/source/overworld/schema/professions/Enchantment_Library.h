#pragma once

#include <overworld/schema/Enchantment.h>
#include "Profession.h"
#include <memory>
#include <unordered_map>
#include <overworld/schema/Enchantment_With_Arguments.h>

namespace overworld {

  class Enchantment_Library {
      std::unordered_map<std::string, Enchantment *> global_enchantments;

  public:
      Enchantment_Library();

      Enchantment *find_enchantment(const std::string &name);
      static Enchantment_With_Parameters &get_external();
      static Enchantment_With_Parameters &get_external_name();
      static Simple_Enchantment &get_static();
      static Simple_Enchantment &get_public();
      static Simple_Enchantment &get_private();
      static Simple_Enchantment &get_protected();
      static Simple_Enchantment &get_value();

      static void initialize();
  };
}