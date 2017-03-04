#pragma once

#include <memory>
#include "Member.h"
#include "professions.h"

namespace underworld {

  class Portal : public virtual Member {
      const std::string name;
      const Profession &profession;

  public:
      Portal(const std::string &name, const Profession &profession) :
        name(name), profession(profession) {}

      Type get_type() const override {
        return Type::portal;
      }
  };

  using Portal_Pointer = std::unique_ptr<Portal>;

}