#pragma once

#include <overworld/schema/professions/Profession.h>
#include "Dungeon.h"

namespace overworld {

  class Dungeon_Reference : public Profession {
      Basic_Dungeon &dungeon;
      Dungeon_Interface_Owner owner;

  public:
      explicit Dungeon_Reference(Basic_Dungeon &dungeon) : dungeon(dungeon) {}

      explicit Dungeon_Reference(Dungeon_Interface_Owner dungeon) :
        dungeon(*dungeon), owner(std::move(dungeon)) {
        int k = 0;
      }

      explicit Dungeon_Reference(const Dungeon &dungeon) : dungeon(const_cast<Dungeon &>(dungeon)) {}
      Dungeon_Reference(const Dungeon_Reference &) = delete;

			~Dungeon_Reference()
			{
				int k = 0;
			}
      Profession_Type get_type() const override {
        return Profession_Type::dungeon;
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

      Basic_Dungeon &get_dungeon_interface() {
        return dungeon;
      }

      const Basic_Dungeon &get_dungeon_interface() const{
        return dungeon;
      }

      Dungeon &get_dungeon() {
        return dungeon.get_original();
      }

      const Basic_Dungeon &get_dungeon() const {
        return dungeon;
      }

      File *get_file() const override {
        return dungeon.get_original().get_file();
      }

      const std::string get_name() const override {
        return dungeon.get_original().get_name();
      }

      const std::string get_debug_name() const override {
        return dungeon.get_debug_name();
      }
  };
}