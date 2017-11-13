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
      Expression_Owner expression;
      std::vector<Expression_Owner> arguments;
//      std::vector<std::unique_ptr<Argument_Node>> argument_nodes;

  public:
      Invoke(Expression_Owner &expression, std::vector<Expression_Owner> &arguments,
             const source_mapping::Source_Range &source_range) :
        Common_Expression(source_range),
        expression(std::move(expression)), arguments(std::move(arguments)) {}

      virtual ~Invoke() override {}

      Expression_Type get_type() const override {
        return Expression_Type::invoke;
      }

      Function &get_function() const {
        auto &member_expression = *dynamic_cast<Member_Expression *>(&expression->get_last());
        auto &member = member_expression.get_member();
        return member.get_function();
      }

//      Function_Signature &get_signature() const {
//        return get_function().get_signature();
//      }

      std::vector<Expression_Owner> &get_arguments() {
        return arguments;
      }

      const std::vector<Expression_Owner> &get_arguments() const {
        return arguments;
      }

      Node *get_node() override {
        throw std::runtime_error("Not supported.");
      }

      const Node *get_node() const override {
        throw std::runtime_error("Not supported.");
      }

      const Expression &get_expression() const {
        return *expression;
      }

      Expression &get_expression() {
        return *expression;
      }

      const std::string get_name() const override {
        return "`Invoke`";
      }

      Profession_Reference &get_profession() override {
        return expression->get_profession();
      }

      const Profession_Reference &get_profession() const override {
        return expression->get_profession();
      }
  };
}