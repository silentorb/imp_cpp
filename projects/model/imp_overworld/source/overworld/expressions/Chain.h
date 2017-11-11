#pragma once

#include "Expression.h"
#include <vector>

namespace overworld {

//  class Chain_Element : public Common_Element {
//      Element &first_element;
//
//  public:
//      Chain_Element(Element &first_element, const std::string &name,
//                    const source_mapping::Source_Range &source_point) :
//        Common_Element(type, name, first_element.get_profession(), source_point),
//        first_element(first_element) {}
//
//      Profession_Reference &get_profession() override {
//        return first_element.get_profession();
//      }
//
//      const Profession_Reference &get_profession() const override {
//        return first_element.get_profession();
//      }
//
//      void set_profession(Profession_Reference &value, Profession_Setter &setter) override {
//        first_element.set_profession(value, setter);
//      }
//  };

  class Chain : public Expression {
      std::vector<Expression_Owner> expressions;

  public:
      Chain(Parent parent, const source_mapping::Source_Range &source_range) {}

      virtual ~Chain() = default;

      void add_expression(Expression_Owner expression) {
        expressions.push_back(std::move(expression));
      }

      Expression_Type get_type() const override {
        return Expression_Type::chain;
      }

      Expression &get_last() override {
        return expressions[expressions.size() - 1]->get_last();
      }

      Node *get_node() override {
        return get_last().get_node();
      }

      const Node *get_node() const override {
        return expressions[expressions.size() - 1]->get_node();
      }

      Expression &get_first() const {
        return *expressions[0];
      }

      const std::vector<Expression_Owner> &get_expressions() const {
        return expressions;
      }

      bool is_statement() const override {
        return false;
      }

      const std::string get_name() const override {
        return "chain";
      }

      Profession_Reference &get_profession() override {
        return get_last().get_profession();
      }

      const Profession_Reference &get_profession() const override {
        return expressions[expressions.size() - 1]->get_profession();
      }
  };
}