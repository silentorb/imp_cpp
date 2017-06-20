#pragma once

#include <memory>
#include <underworld/Source_Point.h>
#include "Member.h"
#include "professions.h"

namespace underworld {

  class Minion : public Member {
      const std::string name;
      const Profession &profession;

  public:
      Minion(const std::string &name, const Profession &profession, const Source_Point &source_point) :
        name(name), profession(profession), Member(source_point) {}

      Type get_type() const override {
        return Type::minion;
      }

      const std::string get_name() const override {
        return name;
      }

      const Profession &get_profession() const override {
        return profession;
      }

  };

  using Minion_Owner = std::unique_ptr<Minion>;

}