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

      Expression_Type get_type() const override {
        return Expression_Type::self;
      }

      const std::string get_name() const override {
        return dungeon.get_name();
      }

      const source_mapping::Source_Range get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}