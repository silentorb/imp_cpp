#pragma once

#include <underworld/schema/Function.h>
#include "Expression.h"

namespace underworld {

  class Lambda : public Common_Expression {
      Function_With_Block_Owner function;

  public:
      Lambda(Function_With_Block_Owner function) : function(std::move(function)) {}

      Expression_Type get_type() const override {
        return Expression_Type::lambda;
      }

      const source_mapping::Source_Range get_source_point() const override {
        return function->get_source_point();
      }

      const Function_With_Block &get_function() const {
        return *function;
      }
  };
}