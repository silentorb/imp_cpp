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
          Else,
          function_call,
          If,
          literal,
          minion,
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

      virtual Node *get_node() {
        throw std::runtime_error("Not implemented.");
      }
  };

//  class Node_Expression : public virtual Expression {
//  protected:
//      Node node;
//
//  public:
//      virtual Node &get_node() override {
//        return node;
//      }
//
//      bool has_node() const override {
//        return true;
//      }
//  };

  using Expression_Owner = std::unique_ptr<Expression>;
  using Expressions = std::vector<Expression_Owner>;
}