#pragma once

#include <overworld/schema/professions/Profession.h>
#include "Dungeon.h"

namespace overworld {

  class Dungeon_Reference : public Profession {
      Dungeon &dungeon;
  public:

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

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }

  };
}