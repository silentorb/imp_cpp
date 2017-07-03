#pragma once

#include <overworld/schema/Minion.h>
#include "Expression.h"
#include <vector>
#include <overworld/schema/Function.h>
#include <underworld/expressions/Invoke.h>

namespace overworld {

  class Invoke : public Common_Expression {
      Function &function;
      std::vector<Expression_Owner> arguments;
      const underworld::Invoke &source;

  public:
      Invoke(Function &function, std::vector<Expression_Owner> &arguments,
                    const underworld::Invoke &source)
        : function(function), arguments(std::move(arguments)), source(source) {}

      virtual ~Invoke() {

      }

      Type get_type() const override {
        return Type::invoke;
      }

      Function &get_function() const {
        return function;
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }

      Node *get_node() override {
        return &function.get_node();
      }
  };
}