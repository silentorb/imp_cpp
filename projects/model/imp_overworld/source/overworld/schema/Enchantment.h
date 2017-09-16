#pragma once

#include <string>
#include <c++/vector>

namespace overworld {

  class Enchantment {

  public:
      virtual const std::string get_name() const = 0;
  };

  class Simple_Enchantment : public Enchantment {
      std::string name;

  public:
      Simple_Enchantment(const std::string &name) : name(name) {}

      const std::string get_name() const override {
        return name;
      }
  };

  class Enchantment_Container {
      std::vector<Enchantment *> enchantments;

  public:
      void add_enchantment(Enchantment &enchantment) {
        if (!has_enchantment(enchantment))
          enchantments.push_back(&enchantment);
      }

      bool has_enchantment(const Enchantment &enchantment) const {
        for (auto &entry : enchantments) {
          if (entry == &enchantment)
            return true;
        }
        return false;
      }
  };
}