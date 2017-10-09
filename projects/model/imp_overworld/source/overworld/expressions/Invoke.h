#pragma once

#include <overworld/schema/Minion.h>
#include "Expression.h"
#include "Member_Expression.h"
#include "Generic_Argument_Node.h"
#include <vector>
#include <overworld/schema/Function.h>
#include <underworld/expressions/Invoke.h>

namespace overworld {

  class Invoke : public Common_Expression {
      Expression_Owner expression;
      std::vector<Expression_Owner> arguments;
      std::vector<std::unique_ptr<Generic_Argument_Node>> argument_nodes;

  public:
      Invoke(Expression_Owner &expression, std::vector<Expression_Owner> &arguments,
             const source_mapping::Source_Range &source_range) :
        Common_Expression(source_range),
        expression(std::move(expression)), arguments(std::move(arguments)) {}

      ~Invoke() override = default;

      Expression_Type get_type() const override {
        return Expression_Type::invoke;
      }

      Function_Signature &get_signature() const {
        auto &member_expression = *dynamic_cast<Member_Expression *>(&expression->get_last());
        auto &member = member_expression.get_member();
        return member.get_function().get_signature();
      }

      std::vector<Expression_Owner> &get_arguments() {
        return arguments;
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }

      Node *get_node() override {
        return &get_signature().get_node();
      }

      const Node *get_node() const override {
        return &get_signature().get_node();
      }

      Expression &get_expression() const {
        return *expression;
      }

      void add_argument_node(std::unique_ptr<Generic_Argument_Node> argument_node) {
        argument_nodes.push_back(std::move(argument_node));
      }

      const std::string get_name() const override {
        return "`Invoke`";
      }

      Profession &get_profession() override {
        return expression->get_profession();
      }

      const Profession &get_profession() const override {
        return expression->get_profession();
      }
  };
}