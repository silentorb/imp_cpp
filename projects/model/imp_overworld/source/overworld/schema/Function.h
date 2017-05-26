#pragma once

#include <memory>
#include <overworld/expressions/Block.h>
#include <underworld/schema/Function.h>
#include "Member.h"

namespace overworld {

  class Function : public virtual Member, public virtual Profession_Reference {
      const underworld::Function &source;
      Function_Block block;
      Scope &parent_scope;
      std::vector<Minion *> parameters;
      Profession_Node<Function> node;
      const Profession *return_type;

  public:
      Function(const underworld::Function &source, const Profession &return_type, Scope &parent_scope) :
        source(source), block(source.get_block(), *this), return_type(&return_type),
        parent_scope(parent_scope), node(*this) {}

      Function_Block &get_block() {
        return block;
      }

      const Function_Block &get_block() const {
        return block;
      }

      Type get_type() const override {
        return Type::function;
      }

      const std::string &get_name() const {
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

      const Profession &get_profession() override {
        return *return_type;
      }

      const Profession &get_profession() const {
        return *return_type;
      }

      void set_profession(const Profession &value) override {
        return_type = &value;
      }

      const underworld::Source_Point &get_source_point() override {
        return source.get_source_point();
      }

      const std::string get_name() override {
        return source.get_name();
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}