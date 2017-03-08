#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"

namespace underworld {

  class Minion_Declaration : public virtual Expression {
  protected:
      Minion &minion;

  public:
      Minion_Declaration(Minion &variable) : minion(variable) {}

      Type get_type() const override {
        return Type::variable_declaration;
      }

      const Minion &get_minion() const {
        return minion;
      }
  };

  class Minion_Declaration_And_Assignment : public virtual Minion_Declaration {
      Expression_Owner expression;

  public:
      Minion_Declaration_And_Assignment(Minion &minion, Expression_Owner &expression) :
        Minion_Declaration(minion), expression(std::move(expression)) {}

      Type get_type() const override {
        return Type::variable_declaration_and_assignment;
      }

      const Expression &get_expression() const {
        return *expression;
      }
  };
}