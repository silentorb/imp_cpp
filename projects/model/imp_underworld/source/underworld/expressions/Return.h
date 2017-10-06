#pragma once

#include "Expression.h"

namespace underworld {

  class Return : public virtual Statement {
  public:
      Expression_Type get_type() const override {
        return Expression_Type::return_nothing;
      }

      const source_mapping::Source_Range get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };


  class Return_With_Value : public virtual Statement {
      Expression_Owner value;

  public:
      Return_With_Value(Expression_Owner &value) :
        value(std::move(value)) {}

      virtual ~Return_With_Value() {

      }

      Expression_Type get_type() const override {
        return Expression_Type::return_with_value;
      }

      const Expression &get_value() const {
        return *value;
      }

      const source_mapping::Source_Range get_source_point() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}