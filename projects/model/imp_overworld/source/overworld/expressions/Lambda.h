#pragma once

#include <overworld/schema/Function.h>
#include "Expression.h"

namespace overworld {

  class Lambda : public Common_Expression {
      Function_With_Block_Owner function;
      Element_Reference_Node node;

  public:
      explicit Lambda(Function_With_Block_Owner function,
                      Function_Interface *outer_function) :
        Common_Expression(function->get_node().get_element().get_source_point()), function(std::move(function)),
        node(element, nullptr, outer_function) {}

      Expression_Type get_type() const override {
        return Expression_Type::lambda;
      }

      const std::string get_name() const override {
        return "Lambda";
      }

      Profession &get_profession() override {
        return function->get_profession();
      }

      const Profession &get_profession() const override {
        return function->get_profession();
      }

      Node *get_node() override {
        return &node;
      }

      const Node *get_node() const override {
        return &node;
      }

      const Function_With_Block &get_function() const {
        return *function;
      }
  };

}