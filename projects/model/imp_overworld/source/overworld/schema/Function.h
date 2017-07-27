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

  class Parameter : public Minion {
  public:
      Parameter(const std::string &name, const Profession &profession) : Minion(name, profession) {}

      bool is_parameter() const override {
        return true;;
      }

      virtual bool transfers_ownership() const {
        return false;
      }
  };

  class Owning_Parameter : public Parameter {
  public:
      Owning_Parameter(const std::string &name, const Profession &profession) :
        Parameter(name, profession) {}

      bool transfers_ownership() const override {
        return true;
      }
  };

  class Function : public virtual Member, public virtual Profession_Reference {
      Function_Scope scope;
      Block block;
      std::vector<Parameter *> parameters;
      Profession_Node <Function> node;
      const Profession *return_type;
      const std::string name;
      bool _is_static = false;
      bool returns_a_value() const;
      const underworld::Source_Point source_point;

  public:
//      Function(const Profession &return_type, Scope &parent_scope) :
//        name(source.get_name()), source(source), return_type(&return_type),
//        scope(source.get_block().get_scope(), parent_scope, *this), block(scope),
//        node(*this) {}

      Function(const std::string &name, const Profession &return_type, Scope &parent_scope,
               const underworld::Source_Point &source_point) :
        name(name), return_type(&return_type),
        scope(parent_scope, *this), block(scope),
        node(*this), source_point(source_point) {}

      Function(const std::string &name, Scope &parent_scope,
               const underworld::Source_Point &source_point) :
        name(name), return_type(nullptr),
        scope(parent_scope, *this), block(scope),
        node(*this), source_point(source_point) {}

      ~Function() {}

      Block &get_block() {
        return block;
      }

      const Block &get_block() const {
        return block;
      }

      Member_Type get_member_type() const override {
        return Member_Type::function;
      }

      const std::string get_name() const override {
        return name;
      }

      bool is_constructor() const;

      bool is_inline();

      const std::vector<Parameter *> &get_parameters() const {
        return parameters;
      }

      void add_parameter(Parameter &minion) {
        parameters.push_back(&minion);
      }

      void add_parameter(Parameter *parameter) {
        scope.add_minion(parameter);
        parameters.push_back(parameter);
      }

//      Minion &create_parameter(const std::string &name, const Profession &profession);

      Profession_Node <Function> &get_node() override {
        return node;
      }

      const Profession &get_profession() const override {
        return *return_type;
      }

      void set_profession(const Profession &value) override {
        return_type = &value;
      }

      const underworld::Source_Point &get_source_point() override {
        return source_point;
      }

      const Function_Scope &get_scope() const {
        return scope;
      }

      Function_Scope &get_scope() {
        return scope;
      }

      bool is_static() const {
        return _is_static;
      }

      void set_is_static(bool value) {
        _is_static = value;
      }

      void finalize(overworld::Profession_Library &profession_library);
  };

  using Function_Owner = std::unique_ptr<Function>;
}