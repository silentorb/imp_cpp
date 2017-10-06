#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"
#include "Member_Expression.h"
#include <vector>
#include <underworld/schema/Function.h>

namespace underworld {

  class Invoke : public Common_Expression {
      Expression_Owner expression;
//      Function &function;
      std::vector<Expression_Owner> arguments;
      source_mapping::Source_Range source;

  public:
      Invoke(Expression_Owner &expression, std::vector<Expression_Owner> &arguments, const source_mapping::Source_Range &source)
        : expression(std::move(expression)), arguments(std::move(arguments)), source(source) {}

      virtual ~Invoke() {

      }

      Expression_Type get_type() const override {
        return Expression_Type::invoke;
      }

//      Function &get_function() const {
//        return function;
//      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }

      const Expression & get_expression()const {
        return *expression;
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source;
      }
  };
}