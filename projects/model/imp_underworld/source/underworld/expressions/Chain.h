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

      virtual ~Chain() {

      }

      Type get_type() const override {
        return Type::chain;
      }

      Expression &get_last() override {
        return second->get_last();
      }

      const Expression &get_last() const override {
        return *second;
      }

      const std::string get_name() const override {
        return "chain";
      }

      const Expression &get_first() const {
        return *first;
      }

      const Expression &get_second() const {
        return *second;
      }

      const source_mapping::Source_Point get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}