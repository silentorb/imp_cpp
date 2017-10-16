#pragma once

#include <overworld/schema/professions/Profession.h>
#include "Dungeon.h"

namespace overworld {

  class Dungeon_Reference : public Profession {
      Dungeon &dungeon;

  public:
      Dungeon_Reference(Dungeon &dungeon) : dungeon(dungeon) {}

      Dungeon_Reference(const Dungeon &dungeon) : dungeon(const_cast<Dungeon &>(dungeon)) {}

      Profession_Type get_type() const override {
        return Profession_Type::dungeon;
      }

      Scope *get_scope() override {
        return dungeon.get_scope();
      }

      const Scope *get_scope() const override {
        return dungeon.get_scope();
      }

      virtual Node &get_node() override {
//        return node;
        throw std::runtime_error("Not supported.");
      }

      Ownership get_ownership() const override {
        return dungeon.get_ownership();
      }

      Profession_Reference &get_base(Profession_Reference &self) override {
        return self;
      }

      const Profession &get_base() const override {
        return *this;
      }

      Dungeon &get_dungeon() {
        return dungeon;
      }

      const Dungeon &get_dungeon() const {
        return dungeon;
      }
  };
}