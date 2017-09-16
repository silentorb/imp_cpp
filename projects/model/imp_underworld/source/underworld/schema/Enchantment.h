#pragma once

#include <string>
#include <memory>
#include <vector>

namespace underworld {
  class Enchantment {
      std::string name;

  public:
      Enchantment(const std::string &name) : name(name) {}

      const std::string &get_name() const {
        return name;
      }
  };

  using Enchantment_Owner = std::unique_ptr<Enchantment>;
  using Enchantment_Array = std::vector<Enchantment_Owner>;
}