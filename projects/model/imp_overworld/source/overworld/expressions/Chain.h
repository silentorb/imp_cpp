#pragma once

#include "Expression.h"
#include <vector>

namespace overworld {

  class Chain : public virtual Expression {
      Expression_Owner first;
      Expression_Owner second;

  public:
      Chain(Expression_Owner &first, Expression_Owner &second) :
        first(std::move(first)), second(std::move(second)) {}

      virtual ~Chain() {

      }

      Type get_type() const override {
        return Type::chain;
      }

      Expression &get_last() override {
        return *second;
      }

      virtual Node *get_node() {
        return second->get_node();
      }

      const Expression_Owner &get_first() const {
        return first;
      }

      const Expression_Owner &get_second() const {
        return second;
      }
//      const std::string get_name() const override {
//        return "chain";
//      }
//
//      const Profession &get_profession() const override {
//        return second->get_profession();
//      }
  };
}