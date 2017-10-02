#pragma once

#include <memory>
#include <vector>
#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Expression {
  public:

      enum class Type {
          assignment,
          block,
          chain,
          Else,
          If,
          instantiation,
          invoke,
          literal,
          member,
          Operator, // Infix
          return_nothing,
          return_with_value,
          self,
          variable_declaration,
          variable_declaration_and_assignment
      };

      virtual Type get_type() const = 0;

//      virtual bool has_node() const {
//        return false;
//      }
      virtual Expression &get_last() = 0;

      virtual Node *get_node() {
        throw std::runtime_error("Not implemented.");
      }

      virtual ~Expression() = default;

      virtual const std::string get_name() const = 0;
      virtual bool is_statement() const = 0;

      virtual Profession &get_profession() = 0;
      virtual const Profession &get_profession() const = 0;

      virtual void set_profession(Profession &value) {
        throw std::runtime_error("Not supported.");
      }
  };

  class Expression_Element : public Element {
      Expression &expression;
      source_mapping::Source_Range source_range;

  public:
      Expression_Element(Expression &expression, const source_mapping::Source_Range &source_range) :
        expression(expression), source_range(source_range) {}

      Profession &get_profession() override {
        return expression.get_profession();
      }

      const Profession &get_profession() const override {
        return expression.get_profession();
      }

      void set_profession(Profession &value) override {
        expression.set_profession(value);
      }

      const source_mapping::Source_Range &get_source_point() const override {
        return source_range;
      }

      const std::string get_name() const override {
        return expression.get_name();
      }

      Element_Type get_type() const override {
        if (expression.get_type() == Expression::Type::instantiation)
          return Element_Type::instantiation;

        return Element_Type::other;
      }
  };

  class Common_Expression : public Expression {
  protected:
      Expression_Element element;

  public:
      Common_Expression(const source_mapping::Source_Range &source_range) :
        element(*this, source_range) {}

      Expression &get_last() override {
        return *this;
      }

      bool is_statement() const override {
        return false;
      }
  };

  class Statement : public Expression {
  public:
      Expression &get_last() override {
        throw std::runtime_error("Not supported");
      }

      bool is_statement() const override {
        return true;
      }

      Type get_type() const override {
        throw std::runtime_error("Not supported.");
      }

      const std::string get_name() const override {
        throw std::runtime_error("Not supported.");
      }

      Profession &get_profession() override {
        throw std::runtime_error("Not supported.");
      }

      const Profession &get_profession() const override {
        throw std::runtime_error("Not supported.");
      }
  };

  using Expression_Owner = std::unique_ptr<Expression>;
  using Expressions = std::vector<Expression_Owner>;
}