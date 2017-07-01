#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"
#include "Member_Expression.h"
#include <vector>
#include <underworld/schema/Function.h>

namespace underworld {

  class Function_Call : public virtual Common_Expression {
      Expression_Owner expression;
//      Function &function;
      std::vector<Expression_Owner> arguments;
      Source_Point source;

  public:
      Function_Call(Expression_Owner &expression, std::vector<Expression_Owner> &arguments, const Source_Point &source)
        : expression(std::move(expression)), arguments(std::move(arguments)), source(source) {}

      virtual ~Function_Call() {

      }

      Type get_type() const override {
        return Type::function_call;
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

      const Profession &get_profession() const override {
        return expression->get_profession();
      }
  };
}