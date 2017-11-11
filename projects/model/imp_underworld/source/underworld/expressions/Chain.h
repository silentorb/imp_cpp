#pragma once

#include "Expression.h"
#include <vector>

namespace underworld {

  class Chain : public Expression {
      std::vector<Expression_Owner> expressions;

  public:
      virtual ~Chain() {}

      Expression_Type get_type() const override {
        return Expression_Type::chain;
      }

      Expression &get_last() override {
        return expressions[expressions.size() - 1]->get_last();
      }

      const Expression &get_last() const override {
        return *expressions[expressions.size() - 1];
      }

      const std::string get_name() const override {
        return "chain";
      }

//      const Expression &get_first() const {
//        return *first;
//      }
//
//      const Expression &get_second() const {
//        return *second;
//      }

      const std::vector<Expression_Owner> &get_expressions() const {
        return expressions;
      }

      void add_expression(Expression_Owner expression) {
        expressions.push_back(std::move(expression));
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_mapping::Source_Range(expressions[0]->get_source_point().get_start(),
                                            get_last().get_source_point().get_end());
      }
  };
}