#pragma once

#include "Expression.h"
#include <vector>

namespace underworld {

  class Chain : public virtual Expression {
      Expression_Owner first;
      Expression_Owner second;

  public:
      Chain(Expression_Owner &first, Expression_Owner &second) :
        first(std::move(first)), second(std::move(second)) {}

      Type get_type() const override {
        return Type::chain;
      }

      Expression &get_last() override {
        return *second;
      }

      const std::string get_name() const override {
        return "chain";
      }

      const Profession &get_profession() const override {
        return second->get_profession();
      }
  };
}