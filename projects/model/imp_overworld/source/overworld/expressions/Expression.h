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

      virtual const Node *get_node() const = 0;
      virtual Node *get_node() = 0;

      virtual ~Expression() = default;

      virtual const std::string get_name() const = 0;
      virtual bool is_statement() const = 0;

      virtual Profession_Reference &get_profession() = 0;
      virtual const Profession_Reference &get_profession() const = 0;

      virtual void set_profession(Profession_Reference &value) {
        throw std::runtime_error("Not supported.");
      }
  };

  class Expression_Element : public Element {
      Expression &expression;
      source_mapping::Source_Range source_range;
      Dungeon_Interface *dungeon = nullptr;
      Function_Interface *function = nullptr;

  public:
      Expression_Element(Expression &expression,
                         Dungeon_Interface *dungeon,
                         Function_Interface *function,
                         const source_mapping::Source_Range &source_range) :
        expression(expression), source_range(source_range) {}

      Dungeon_Interface *get_dungeon() const override {
        return nullptr;
      }

      Function_Interface *get_function() const override {
        return nullptr;
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
  protected:
      Expression_Element element;

  public:
      Common_Expression(Dungeon_Interface *dungeon,
                        Function_Interface *function, const source_mapping::Source_Range &source_range) :
        element(*this, dungeon, function, source_range) {}

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

      const Profession_Reference &get_profession() const override {
        throw std::runtime_error("Not supported.");
      }
  };

  using Expression_Owner = std::unique_ptr<Expression>;
  using Expressions = std::vector<Expression_Owner>;
}