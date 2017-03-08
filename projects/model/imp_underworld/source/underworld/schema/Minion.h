#pragma once

#include <memory>
#include "Member.h"
#include "professions.h"

namespace underworld {

  class Minion : public virtual Member {
      const std::string name;
      const Profession &profession;

  public:
      Minion(const std::string &name, const Profession &profession) :
        name(name), profession(profession) {}

      Type get_type() const override {
        return Type::variable;
      }

      const std::string &get_name() const {
        return name;
      }

      const Profession &get_profession() const {
        return profession;
      }
  };

  using Minion_Owner = std::unique_ptr<Minion>;

}