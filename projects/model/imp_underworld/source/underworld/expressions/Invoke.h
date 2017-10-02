#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"
#include "Member_Expression.h"
#include <vector>
#include <underworld/schema/Function.h>

namespace underworld {

  class Invoke : public virtual Common_Expression {
      Expression_Owner expression;
//      Function &function;
      std::vector<Expression_Owner> arguments;
      source_mapping::Source_Point source;

  public:
      Invoke(Expression_Owner &expression, std::vector<Expression_Owner> &arguments, const source_mapping::Source_Point &source)
        : expression(std::move(expression)), arguments(std::move(arguments)), source(source) {}

      virtual ~Invoke() {

      }

      Type get_type() const override {
        return Type::invoke;
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

      const source_mapping::Source_Point get_source_point() const override {
        return source;
      }
  };
}