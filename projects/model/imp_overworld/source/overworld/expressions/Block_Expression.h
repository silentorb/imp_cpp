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

  class Block_Expression : public virtual Expression, public Scope {
      Block block;

  public:
      Block_Expression(const underworld::Scope &source, Scope *parent) :
        Scope(&source, parent), block(*this) {}

      virtual ~Block_Expression() {

      }

      void add_expression(Expression_Owner &expression) {
        block.add_expression(expression);
      }

      const std::vector<Expression_Owner> &get_expressions() const {
        return block.get_expressions();
      }

      Type get_type() const override {
        return Type::block;
      }

      const Block &get_block() const {
        return block;
      }

       Block &get_block()  {
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

//      Scope_Parent_Type get_scope_parent_type() const override {
//        return Scope_Parent_Type::block;
//      }
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