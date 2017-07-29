#pragma once

#include "Expression.h"
#include <vector>

namespace overworld {

  class Chain : public virtual Expression {
      Expression_Owner first;
      Expression_Owner second;

  public:
      Chain(Expression_Owner &first, Expression_Owner &second) :
        first(std::move(first)), second(std::move(second)) {}

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

      Expression &get_first() const{
        return *first;
      }

      Expression &get_second() const{
        return *second;
      }
//      const std::string get_name() const override {
//        return "chain";
//      }
//
//      Profession &get_profession() const override {
//        return second->get_profession();
//      }
  };
}