#pragma once

#include <memory>
#include "Member.h"
#include "professions.h"
#include <underworld/schema/Minion.h>

namespace overworld {

  class Minion : public virtual Member {
      const underworld::Minion &source;
      const Profession &profession;

  public:
      Minion(const underworld::Minion &source, const Profession &profession) :
        source(source), profession(profession) {}

      Type get_type() const override {
        return Type::variable;
      }

      const std::string &get_name() const {
        return source.get_name();
      }
  };

  using Variable_Owner = std::unique_ptr<Minion>;

}