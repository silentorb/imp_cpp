#pragma once

#include "Expression.h"
#include "../schema/Dungeon.h"

namespace underworld {

  class Self : public Common_Expression {
      Dungeon &dungeon;

  public:
      Self(Dungeon &dungeon) : dungeon(dungeon) {}

      virtual ~Self() {

      }

      Type get_type() const override {
        return Type::self;
      }

      const std::string get_name() const override {
        return dungeon.get_name();
      }

  };
}