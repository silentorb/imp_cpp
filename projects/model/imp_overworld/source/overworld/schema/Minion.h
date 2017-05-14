#pragma once

#include <memory>
#include "Member.h"
#include "professions.h"
#include <underworld/schema/Minion.h>

namespace overworld {

  class Minion : public virtual Member, public virtual Profession_Reference {
      const underworld::Minion &source;
      const Profession *profession;

  public:
      Minion(const underworld::Minion &source, const Profession &profession) :
        source(source), profession(&profession) {}

      Type get_type() const override {
        return Type::variable;
      }

      const std::string &get_name() const {
        return source.get_name();
      }

      const Profession &get_profession() const {
        return *profession;
      }

      const Profession &get_profession() override {
        return *profession;
      }

      void set_profession(const Profession &value) override {
        profession = &value;
      }
  };

  using Variable_Owner = std::unique_ptr<Minion>;

}