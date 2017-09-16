#pragma once

#include <overworld/schema/Minion.h>
#include "Expression.h"
#include "Member_Expression.h"
#include "Argument_Node.h"
#include <vector>
#include <overworld/schema/Function.h>
#include <underworld/expressions/Invoke.h>

namespace overworld {

  class Invoke : public Common_Expression {
//      Function_Signature &signature;
      Expression_Owner expression;
      std::vector<Expression_Owner> arguments;
      const underworld::Invoke &source;
      std::vector<std::unique_ptr<Argument_Node>> argument_nodes;

  public:
      Invoke(Expression_Owner &expression, std::vector<Expression_Owner> &arguments,
             const underworld::Invoke &source)
        : expression(std::move(expression)), arguments(std::move(arguments)),
          source(source) {}

      virtual ~Invoke() {

      }

      Type get_type() const override {
        return Type::invoke;
      }

      Function_Signature &get_signature() const {
        auto &member_expression = *dynamic_cast<Member_Expression *>(&expression->get_last());
        auto &member = static_cast<Member_Function &>(member_expression.get_member());
        return member.get_function().get_signature();
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }

      Node *get_node() override {
        return &get_signature().get_node();
      }

      Expression &get_expression() const {
        return *expression;
      }

      void add_argument_node(std::unique_ptr<Argument_Node> argument_node) {
        argument_nodes.push_back(std::move(argument_node));
      }
  };
}