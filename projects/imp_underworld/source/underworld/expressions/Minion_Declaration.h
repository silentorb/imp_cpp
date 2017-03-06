#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"

namespace underworld {

  class Minion_Declaration : public virtual Expression {
  protected:
      Minion &minion;

  public:
      Minion_Declaration(Minion &variable) : minion(variable) {}

      Type get_type() override {
        return Type::variable_declaration;
      }
  };

  class Minion_Declaration_And_Assignment : public virtual Minion_Declaration {
      Expression_Owner expression;

  public:
      Minion_Declaration_And_Assignment(Minion &minion, Expression_Owner &expression) :
        Minion_Declaration(minion), expression(std::move(expression)) {}

      Type get_type() override {
        return Type::variable_declaration_and_assignment;
      }
  };
}