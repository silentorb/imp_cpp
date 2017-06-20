#pragma once

#include <overworld/imp_graph/Node.h>
#include "overworld/schema/Dungeon.h"
#include "Expression.h"

namespace overworld {

  class Self : public virtual Expression {
      Dungeon &dungeon;

  public:
      Self(Dungeon &dungeon) : dungeon(dungeon) {}

      Type get_type() const override {
        return Type::self;
      }

      Node *get_node() override {
        return &dungeon.get_node();
      }
  };
}