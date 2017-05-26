#pragma once

#include <overworld/schema/Minion.h>
#include "Expression.h"
#include <vector>
#include <overworld/schema/Function.h>
#include <underworld/expressions/Function_Call.h>

namespace overworld {

  class Function_Call : public virtual Expression {
      Function &function;
      std::vector<Expression_Owner> arguments;
      const underworld::Function_Call &source;

  public:
      Function_Call(Function &function, std::vector<Expression_Owner> &arguments,
                    const underworld::Function_Call &source)
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