#pragma once

#include "Expression.h"
#include <vector>

namespace underworld {

  class Chain : public Expression {
      Expression_Owner first;
      Expression_Owner second;

  public:
      Chain(Expression_Owner &first, Expression_Owner &second) :
        first(std::move(first)), second(std::move(second)) {}

      virtual ~Chain() {

      }

      Expression_Type get_type() const override {
        return Expression_Type::chain;
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

      const source_mapping::Source_Range get_source_point() const override {
        return source_mapping::Source_Range(first->get_source_point().get_start(),
                                            second->get_source_point().get_end());
      }
  };
}