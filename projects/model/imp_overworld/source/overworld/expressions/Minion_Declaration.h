#pragma once

#include <overworld/schema/Minion.h>
#include "Expression.h"

namespace overworld {

  class Minion_Declaration : public Statement {
  protected:
      Minion &minion;

  public:
      Minion_Declaration(Minion &variable) : minion(variable) {}

      virtual ~Minion_Declaration() {

      }

      Expression_Type get_type() const override {
        return Expression_Type::variable_declaration;
      }

      const Minion &get_minion() const {
        return minion;
      }
  };

  class Minion_Declaration_And_Assignment : public Minion_Declaration {
      Expression_Owner expression;

  public:
      Minion_Declaration_And_Assignment(Minion &minion, Expression_Owner &expression) :
        Minion_Declaration(minion), expression(std::move(expression)) {}

      Expression_Type get_type() const override {
        return Expression_Type::variable_declaration_and_assignment;
      }

      const Expression &get_expression() const {
        return *expression;
      }
  };
}