#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"
#include <vector>
#include <underworld/schema/Function.h>

namespace underworld {

  class Function_Call : public virtual Common_Expression {
      Function &function;
      std::vector<Expression_Owner> arguments;
      Source_Point source;

  public:
      Function_Call(Function &function, std::vector<Expression_Owner> &arguments, const Source_Point &source)
        : function(function), arguments(std::move(arguments)), source(source) {}

      Type get_type() const override {
        return Type::function_call;
      }

      Function &get_function() const {
        return function;
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }
  };
}