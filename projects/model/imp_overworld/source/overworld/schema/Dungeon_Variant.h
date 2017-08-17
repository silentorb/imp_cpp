#pragma once

#include <overworld/schema/Dungeon.h>

namespace overworld {

  class Dungeon_Variant : public virtual Profession {
      Dungeon &original;
      std::vector<Profession *> professions;
      std::string name;

      void format_name() {
        name = original.get_name() + "<";
        auto first = true;
        for (auto profession : professions) {
          if (first)
            first = false;
          else
            name += ", ";

          name += profession->get_name();
        }
        name += ">";
      }

  public:
      Dungeon_Variant(Dungeon &original, const std::vector<Profession *> &professions) :
        original(original), professions(professions) {
        format_name();
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
  };

  using Dungeon_Variant_Owner = std::unique_ptr<Dungeon_Variant>;
}