#pragma once

#include <underworld/schema/Minion.h>
#include "Expression.h"

namespace underworld {

  class Minion_Declaration : public virtual Statement {
  protected:
      Minion_Owner minion;

  public:
      Minion_Declaration(Minion_Owner &variable) : minion(std:: move(variable)) {}

      virtual ~Minion_Declaration() {

      }

      Type get_type() const override {
        return Type::variable_declaration;
      }

      const Minion &get_minion() const {
        return* minion;
      }

       Minion &get_minion()  {
        return* minion;
      }
  };

  class Minion_Declaration_And_Assignment : public virtual Minion_Declaration {
      Expression_Owner expression;

  public:
      Minion_Declaration_And_Assignment(Minion_Owner &minion, Expression_Owner &expression) :
        Minion_Declaration(minion), expression(std::move(expression)) {}

      virtual ~Minion_Declaration_And_Assignment() {

      }

      Type get_type() const override {
        return Type::variable_declaration_and_assignment;
      }

      const Expression &get_expression() const {
        return *expression;
      }
  };
}