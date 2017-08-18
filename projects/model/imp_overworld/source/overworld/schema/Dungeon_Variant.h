#pragma once

#include <overworld/schema/Dungeon.h>
#include "Variant.h"

namespace overworld {

  class Dungeon_Variant : public Variant, public virtual Dungeon_Interface {
      Dungeon &original;

  public:
      Dungeon_Variant(Dungeon &original, const std::vector<Profession *> &professions) :
        Variant(original.get_name(), professions),
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
        return name;
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

      const std::vector<Profession *> &get_professions() const {
        return professions;
      }

      Dungeon &get_original() {
        return original;
      }

      Node &add_node(Node_Owner node) {
        auto &result = *node;
        nodes.push_back(std::move(node));
        return result;
      }
  };

  using Dungeon_Variant_Owner = std::unique_ptr<Dungeon_Variant>;
}