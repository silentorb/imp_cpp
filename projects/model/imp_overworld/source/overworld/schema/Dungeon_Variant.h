#pragma once

#include <overworld/schema/Dungeon.h>
#include "Variant.h"

namespace overworld {

  class Dungeon_Variant : public Variant, public virtual Dungeon_Interface {
      Dungeon &original;

  public:
      Dungeon_Variant(Dungeon &original, std::vector<Profession *> &professions) :
        Variant(original.get_name(), original.get_generic_parameters(), professions),
        original(original) {
      }

      Profession_Type get_type() const override {
        return Profession_Type::variant;
      }

      Scope *get_scope() override {
        return original.get_scope();
      }

      File *get_file() const override {
        return original.get_file();
      }

      const std::string get_name() const override {
        return Variant::get_name();
      }

      Ownership get_ownership() const override {
        return original.get_ownership();
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }

      Dungeon &get_original() override {
        return original;
      }

      const Dungeon &get_original() const {
        return original;
      }

      Dungeon_Type get_dungeon_type() const override {
        return Dungeon_Type::variant;
      }

  };

  using Dungeon_Variant_Owner = std::unique_ptr<Dungeon_Variant>;
}