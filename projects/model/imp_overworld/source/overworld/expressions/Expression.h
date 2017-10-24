#pragma once

#include <memory>
#include <vector>
#include <overworld/imp_graph/Node.h>

namespace overworld {

  enum class Expression_Type {
      assignment,
      block,
      chain,
      Else,
      If,
      instantiation,
      invoke,
      lambda,
      literal,
      member,
      Operator, // Infix
      range,
      return_nothing,
      return_with_value,
      self,
      variable_declaration,
      variable_declaration_and_assignment
  };

  class Expression {
  public:

      virtual Expression_Type get_type() const = 0;

//      virtual bool has_node() const {
//        return false;
//      }
      virtual Expression &get_last() = 0;
      const Expression &get_last() const {
        return const_cast<Expression*>(this)->get_last();
      }

      virtual const Node *get_node() const = 0;
      virtual Node *get_node() = 0;

      virtual ~Expression() = default;

      virtual const std::string get_name() const = 0;
      virtual bool is_statement() const = 0;

      virtual Profession_Reference &get_profession() = 0;
      virtual const Profession &get_profession() const = 0;

//      virtual void set_profession(Profession_Reference &value) {
//        throw std::runtime_error("Not supported.");
//      }
  };

  class Expression_Element : public Element {
      Expression &expression;
      source_mapping::Source_Range source_range;
      Parent parent;

  public:
      Expression_Element(Expression &expression,
                         Parent parent,
                         const source_mapping::Source_Range &source_range) :
        expression(expression), source_range(source_range), parent(parent) {}

      Parent &get_parent() override {
        return parent;
      }

      const source_mapping::Source_Range &get_source_point() const override {
        return source_range;
      }

      const std::string get_name() const override {
        return expression.get_name();
      }

      Element_Type get_type() const override {
        if (expression.get_type() == Expression_Type::instantiation)
          return Element_Type::instantiation;

        return Element_Type::other;
      }
  };

  class Common_Expression : public Expression {
      const source_mapping::Source_Range &source_range;

  public:
      Common_Expression(const source_mapping::Source_Range &source_range) :
        source_range(source_range) {}

      Expression &get_last() override {
        return *this;
      }

      bool is_statement() const override {
        return false;
      }
  };

  class Common_Node_Expression : public Expression {
  protected:
      Expression_Element element;

  public:
      Common_Node_Expression(Parent parent, const source_mapping::Source_Range &source_range) :
        element(*this, parent, source_range) {}

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

      const Node *get_node() const override {
        throw std::runtime_error("Not implemented.");
      }

      Node *get_node() override {
        throw std::runtime_error("Not implemented.");
      }

      Expression_Type get_type() const override {
        throw std::runtime_error("Not supported.");
      }

      const std::string get_name() const override {
        throw std::runtime_error("Not supported.");
      }

      Profession_Reference &get_profession() override {
        throw std::runtime_error("Not supported.");
      }

      const Profession &get_profession() const override {
        throw std::runtime_error("Not supported.");
      }
  };

  using Expression_Owner = std::unique_ptr<Expression>;
  using Expressions = std::vector<Expression_Owner>;
}