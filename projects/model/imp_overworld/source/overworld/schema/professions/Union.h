#pragma once

#include "Profession.h"

namespace overworld {

  class Union : public Profession {
      std::vector<Profession *> professions;

  public:
      Union(const std::vector<Profession *> &professions) : professions(professions) {}

      Profession_Type get_type() const override {
        return Profession_Type::Union;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }

      const std::string get_name() const override {
        return "Union";
      }

      Ownership get_ownership() const override {
        return Ownership::unknown;
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }
  };
}