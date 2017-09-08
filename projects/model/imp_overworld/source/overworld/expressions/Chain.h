#pragma once

#include "Expression.h"
#include <vector>

namespace overworld {

  class Chain : public Expression, public Profession_Reference {
      Expression_Owner first;
      Expression_Owner second;
      Profession_Node <Chain> node;

  public:
      Chain(Expression_Owner &first, Expression_Owner &second, Dungeon_Interface *dungeon,
            Function_Interface *function) :
        first(std::move(first)), second(std::move(second)),
        node(*this, this->first->get_node()->get_profession(), dungeon, function) {}

      virtual ~Chain() {

      }

      Type get_type() const override {
        return Type::chain;
      }

      Expression &get_last() override {
        return second->get_last();
      }

      virtual Node *get_node() {
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

      Profession &get_profession() override {
        return first->get_node()->get_profession();
      }

      const Profession &get_profession() const override {
        return first->get_node()->get_profession();
      }

      void set_profession(Profession &value) override {
        first->get_node()->set_profession(value);
      }

      const source_mapping::Source_Point &get_source_point() const override {
        return second->get_node()->get_profession_reference().get_source_point();
      }

      const std::string get_name() const override {
        return "chain";
      }
  };
}