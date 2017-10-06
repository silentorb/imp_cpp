#pragma once

#include "Enchantment.h"

namespace overworld {

  class Enchantment_Container {
      std::vector<Enchantment_Owner> enchantments;

  public:
      Enchantment_Container() = default;

      Enchantment_Container(const Enchantment_Container &) = delete;

      void add_enchantment(Enchantment_Owner enchantment) {
//        if (!has_enchantment(*enchantment))
          enchantments.push_back(std::move(enchantment));
      }

      void add_enchantment(Dungeon &dungeon) {
        if (!has_enchantment(dungeon))
          enchantments.push_back(Enchantment_Owner(new Enchantment(dungeon)));
      }

      Enchantment *get_enchantment(const Dungeon &dungeon) const {
        for (auto &entry : enchantments) {
          if (&entry->get_dungeon() == &dungeon)
            return entry.get();
        }
        return nullptr;
      }

      bool has_enchantment(const Dungeon &enchantment) const {
        return get_enchantment(enchantment) != nullptr;
      }
  };
}