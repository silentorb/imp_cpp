#pragma once

#include <overworld/imp_graph/Node.h>
#include "overworld/schema/Dungeon.h"
#include "Expression.h"

namespace overworld {

  class Self : public Common_Expression {
      Dungeon &dungeon;

  public:
      Self(Dungeon &dungeon, const source_mapping::Source_Range &source_range) :
        Common_Expression(source_range), dungeon(dungeon) {}

      ~Self() override = default;

      Expression_Type get_type() const override {
        return Expression_Type::self;
      }

      Node *get_node() override {
        throw std::runtime_error("Not implemented.");
//        return &dungeon.get_node();
      }

      const Node *get_node() const override {
        throw std::runtime_error("Not implemented.");
//        return &dungeon.get_node();
      }

      const std::string get_name() const override {
        throw std::runtime_error("Not supported.");
      }

      Profession_Reference &get_profession() override {
        throw std::runtime_error("Not supported.");
      }

      const Profession &get_profession() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}