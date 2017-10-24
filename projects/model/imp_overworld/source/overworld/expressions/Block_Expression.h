#pragma once

#include <vector>
#include "overworld/schema/Scope.h"
#include "Expression.h"

namespace overworld {

  class Block {
      Scope &scope;
      std::vector<Expression_Owner> expressions;

  public:
      Block(Scope &scope) : scope(scope) {}

      void add_expression(Expression_Owner &expression) {
        expressions.push_back(std::move(expression));
      }

      const std::vector<Expression_Owner> &get_expressions() const {
        return expressions;
      }

      Scope &get_scope() {
        return scope;
      }

      const Scope &get_scope() const {
        return scope;
      }

  };

  class Block_Expression : public Statement {
      Scope scope;
      Block block;

  public:
      Block_Expression(Scope *parent) :
        scope(parent, Parent()), block(scope) {}

      virtual ~Block_Expression() override = default;

      void add_expression(Expression_Owner &expression) {
        block.add_expression(expression);
      }

      const std::vector<Expression_Owner> &get_expressions() const {
        return block.get_expressions();
      }

      Expression_Type get_type() const override {
        return Expression_Type::block;
      }

      const Block &get_block() const {
        return block;
      }

      Block &get_block() {
        return block;
      }

      Scope &get_scope() {
        return block.get_scope();
      }

      const Scope &get_scope() const {
        return block.get_scope();
      }

      Expression &get_last() override {
        throw std::runtime_error("Not supported");
      }

      bool is_statement() const override {
        return false;
      }
  };


//  class Basic_Block : public virtual Block_Expression {
//      Scope scope;
//
//  public:
//      Basic_Block(const underworld::Block &source) :
//        scope(source.get_scope(), this) {}
//
//      virtual Scope &get_scope() override {
//        return scope;
//      }
//
//      virtual const Scope &get_scope() const override {
//        return scope;
//      }
//
//  };

//  class Function_Block : public virtual Block_Expression {
//      Function_Scope scope;
//
//  public:
//      Function_Block(const underworld::Block &source, Function &function) :
//        scope(source.get_scope(), *this, function) {}
//
//      virtual Function_Scope &get_scope() override {
//        return scope;
//      }
//
//      virtual const Function_Scope &get_scope() const override {
//        return scope;
//      }
//
//  };
}