#pragma once

#include <overworld/schema/Variable.h>
#include "Expression.h"

namespace overworld {

  class Minion_Declaration : public virtual Expression {
  protected:
      Variable &minion;

  public:
      Minion_Declaration(Variable &variable) : minion(variable) {}

      Type get_type() const override {
        return Type::variable_declaration;
      }
  };

  class Minion_Declaration_And_Assignment : public virtual Minion_Declaration {
      Expression_Owner expression;

  public:
      Minion_Declaration_And_Assignment(Variable &minion, Expression_Owner &expression) :
        Minion_Declaration(minion), expression(std::move(expression)) {}

      Type get_type() const override {
        return Type::variable_declaration_and_assignment;
      }
  };
}