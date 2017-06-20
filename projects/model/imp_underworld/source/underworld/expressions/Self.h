#pragma once

#include "Expression.h"
#include "../schema/Dungeon.h"

namespace underworld {

  class Self : public virtual Expression {
      Dungeon &dungeon;

  public:
      Self(Dungeon &dungeon) : dungeon(dungeon) {}

      Type get_type() const override {
        return Type::self;
      }

      Expression &get_last() override {
        return *this;
      }

      const std::string get_name() const override {
        return dungeon.get_name();
      }

      const Profession &get_profession() const override {
        return dungeon;
      }
  };
}