#pragma once

#include <memory>
#include <underworld/Source_Point.h>
#include "Member.h"
#include "professions.h"

namespace underworld {

  class Minion : public virtual Member {
      const std::string name;
      const Profession &profession;
      Source_Point source;

  public:
      Minion(const std::string &name, const Profession &profession, const Source_Point &source) :
        name(name), profession(profession), source(source) {}

      Type get_type() const override {
        return Type::variable;
      }

      const std::string &get_name() const {
        return name;
      }

      const Profession &get_profession() const {
        return profession;
      }

      const Source_Point &get_source_point() const {
        return source;
      }
  };

  using Minion_Owner = std::unique_ptr<Minion>;

}