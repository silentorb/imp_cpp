#pragma once

#include <memory>
#include "../schema/Profession.h"

namespace underworld {

  class Expression {
  public:

      enum class Type {
          assignment,
          block,
          chain,
          Else,
          instantiation,
          invoke,
          If,
          literal,
          member,
          method_call,
          Operator, // Infix
          return_nothing,
          return_with_value,
          self,
//          unresolved_member,
          variable_declaration,
          variable_declaration_and_assignment
      };

      virtual Type get_type() const = 0;
      virtual Expression &get_last() = 0;
      virtual const Expression &get_last() const = 0;
      virtual const std::string get_name() const = 0;

      virtual ~Expression() {

      }
  };

  class Common_Expression : public virtual Expression {
  public:
      Expression &get_last() override {
        return *this;
      }

      const Expression &get_last() const override {
        return *this;
      }

      const std::string get_name() const override {
        return "Expression";
      }
  };

  class Statement : public virtual Expression {
  public:
      Expression &get_last() override {
        throw std::runtime_error("Not supported");
      }

      const Expression &get_last() const override {
        throw std::runtime_error("Not supported");
      }

      const std::string get_name() const override {
        return "Expression";
      }
  };

  using Expression_Owner = std::unique_ptr<Expression>;
}