#pragma once

#include <memory>
#include <overworld/expressions/Block_Expression.h>
#include <underworld/schema/Function.h>
#include "Member.h"

namespace overworld {
  class Profession_Library;

//  class Function_Node : public Profession_Node<Function> {
//  public:
//      Function_Node::Function_Node(Function &element) : Profession_Node(element) {}
//
//      bool returns_a_value() {
//
//      }
//
//      void set_profession(const Profession &value) override {
//        Node::set_profession(value);
//      }
//  };

  class Function : public virtual Member, public virtual Profession_Reference {
      const underworld::Function &source;
      Function_Scope scope;
      Block block;
      std::vector<Minion *> parameters;
      Profession_Node<Function> node;
      const Profession *return_type;
      bool is_static = false;
      bool returns_a_value() const;

  public:
      Function(const underworld::Function &source, const Profession &return_type, Scope &parent_scope) :
        source(source), return_type(&return_type),
        scope(source.get_block().get_scope(), parent_scope, *this), block(scope),
        node(*this) {}

      Block &get_block() {
        return block;
      }

      const Block &get_block() const {
        return block;
      }

      Type get_type() const override {
        return Type::function;
      }

      const std::string get_name() const override{
        return source.get_name();
      }

      bool is_constructor() const;

      bool is_inline();

      const std::vector<Minion *> &get_parameters() const {
        return parameters;
      }

      void add_parameter(Minion &minion) {
        parameters.push_back(&minion);
      }

      Profession_Node<Function> &get_node() {
        return node;
      }

      const Profession &get_profession() const override{
        return *return_type;
      }

      void set_profession(const Profession &value) override {
        return_type = &value;
      }

      const underworld::Source_Point &get_source_point() override {
        return source.get_source_point();
      }

      void finalize(overworld::Profession_Library &profession_library);
  };

  using Function_Owner = std::unique_ptr<Function>;
}