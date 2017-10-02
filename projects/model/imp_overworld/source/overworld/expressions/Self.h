#pragma once

#include <overworld/imp_graph/Node.h>
#include "overworld/schema/Dungeon.h"
#include "Expression.h"

namespace overworld {

  class Self : public Common_Expression {
      Dungeon &dungeon;

  public:
      Self(Dungeon &dungeon, const source_mapping::Source_Range &source_range) :
        Common_Expression(source_range),                                                                                 dungeon(dungeon) {}

      ~Self() override = default;

      Type get_type() const override {
        return Type::self;
      }

      Node *get_node() override {
        return &dungeon.get_node();
      }
  };
}