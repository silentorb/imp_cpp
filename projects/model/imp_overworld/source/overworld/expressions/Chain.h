#pragma once

#include "Expression.h"
#include <vector>

namespace overworld {


  class Chain_Element : public Common_Element {
      Element &first_element;

  public:
      Chain_Element(Element &first_element, const std::string &name,
                    const source_mapping::Source_Range &source_point) :
        Common_Element(type, name, first_element.get_profession(), source_point),
        first_element(first_element) {}

      Profession &get_profession() override {
        return first_element.get_profession();
      }

      const Profession &get_profession() const override {
        return first_element.get_profession();
      }

      void set_profession(Profession &value, Profession_Setter &setter) override {
        first_element.set_profession(value, setter);
      }
  };

  class Chain : public Expression {
      Chain_Element element;
      Expression_Owner first;
      Expression_Owner second;
      Element_Reference_Node node;

  public:
      Chain(Expression_Owner &first, Expression_Owner &second, Dungeon_Interface *dungeon,
            Function_Interface *function, const source_mapping::Source_Range &source_range) :
        element(first->get_node()->get_element(), "chain", source_range),
        first(std::move(first)), second(std::move(second)),
        node(element, dungeon, function) {}

      virtual ~Chain() = default;

      Expression_Type get_type() const override {
        return Expression_Type::chain;
      }

      Expression &get_last() override {
        return second->get_last();
      }

      Node *get_node() override {
        return second->get_node();
      }

      const Node *get_node() const override {
        return second->get_node();
      }

      Expression &get_first() const {
        return *first;
      }

      Expression &get_second() const {
        return *second;
      }

      bool is_statement() const override {
        return false;
      }

      const std::string get_name() const override {
        return "chain";
      }

      Profession &get_profession() override {
        return second->get_profession();
      }

      const Profession &get_profession() const override {
        return second->get_profession();
      }
  };
}