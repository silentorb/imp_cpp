#pragma once

#include <overworld/schema/professions/Profession.h>
#include "Dungeon.h"

namespace overworld {

  class Dungeon_Reference : public Profession {
      Dungeon &dungeon;
      Dungeon_Interface_Owner dungeon_owner;

  public:
      explicit Dungeon_Reference(Dungeon &dungeon) : dungeon(dungeon) {}

      explicit Dungeon_Reference(Dungeon_Interface_Owner dungeon) :
        dungeon(dungeon->get_original()), dungeon_owner(std::move(dungeon_owner)) {}

      explicit Dungeon_Reference(const Dungeon &dungeon) : dungeon(const_cast<Dungeon &>(dungeon)) {}

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

      File *get_file() const override {
        return dungeon.get_file();
      }

      const std::string get_name() const override {
        return dungeon.get_name();
      }
  };
}