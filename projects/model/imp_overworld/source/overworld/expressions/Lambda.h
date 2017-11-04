#pragma once

#include <overworld/schema/Function.h>
#include "Expression.h"

namespace overworld {

  class Lambda : public Common_Node_Expression {
      Function_With_Block_Owner function;

  public:
      explicit Lambda(Function_With_Block_Owner function,
                      Parent parent) :
        Common_Node_Expression(Parent(*function), Profession_Reference(function->get_signature()),
                               function->get_element().get_source_point()),
        function(std::move(function)) {}

      Expression_Type get_type() const override {
        return Expression_Type::lambda;
      }

      const std::string get_name() const override {
        auto signature = static_cast<const Function_Signature *>(node.get_profession().get());
        return "L" + signature->get_debug_name_with_names();
      }

      Profession_Reference &get_profession() override {
        return node.get_profession();
      }

      const Profession_Reference &get_profession() const override {
        return node.get_profession();
      }

      const Function_With_Block &get_function() const {
        return *function;
      }
  };

}