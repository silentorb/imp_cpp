#pragma once

#include <overworld/schema/Function.h>
#include "Expression.h"

namespace overworld {

  class Lambda : public Common_Node_Expression {
      Function_With_Block_Owner function;
      Node node;

  public:
      explicit Lambda(Function_With_Block_Owner function,
                      Parent parent) :
        Common_Node_Expression(Parent(*function), function->get_element().get_source_point()),
        function(std::move(function)),
        node(this->function->get_profession_reference(), element) {}

      Expression_Type get_type() const override {
        return Expression_Type::lambda;
      }

      const std::string get_name() const override {
        return "Lambda";
      }

      Profession_Reference &get_profession() override {
        return node.get_profession();
      }

      const Profession &get_profession() const override {
        return node.get_profession();
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