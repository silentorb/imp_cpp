#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"
#include <vector>
#include <underworld/schema/Function.h>

namespace underworld {

  class Function_Call : public virtual Expression {
      Function &function;
      std::vector<Expression_Owner> arguments;
      Source_Point source;

  public:
      Function_Call(Function &function, std::vector<Expression_Owner> &arguments, const Source_Point &source)
        : function(function), arguments(std::move(arguments)), source(source) {}

      Type get_type() const override {
        return Type::function_call;
      }
  };
}