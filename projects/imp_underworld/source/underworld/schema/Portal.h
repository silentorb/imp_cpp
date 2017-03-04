#pragma once

#include <memory>
#include "Member.h"

namespace underworld {

  class Portal : public virtual Member{
      const std::string name;

  public:
      Portal(const std::string &name) : name(name) {}

      Type get_type() const override {
        return Type::portal;
      }
  };

  using Portal_Pointer = std::unique_ptr<Portal>;

}