#pragma once

#include <memory>
#include <overworld/expressions/Block_Expression.h>
#include <overworld/schema/professions/Profession_Library.h>
#include "Parameter.h"

namespace overworld {

  class Profession_Library;

  class Function_Signature {
      std::vector<Parameter *> parameters;
      Node &node;
      Profession *return_type = nullptr;

  public:

      const std::vector<Parameter *> &get_parameters() const {
        return parameters;
      }

      Function_Signature(Node &node, Profession *return_type = nullptr) :
        node(node), return_type(return_type) {}

      Profession *get_return_type() const {
        return return_type;
      }

      void add_parameter(Parameter &parameter) {
        parameters.push_back(&parameter);
      }

      void set_return_type(Profession &profession) {
        return_type = &profession;;
      }

      Node &get_node() {
        return node;
      }
  };

  class Function : public virtual Member, public virtual Profession_Reference {
      Function_Scope scope;
      Block block;
      Function_Signature signature;
      Profession_Node<Function> node;
      const std::string name;
      bool _is_static = false;
      bool returns_a_value() const;
      const underworld::Source_Point source_point;

  public:
      Function(const std::string &name, Profession &return_type, Scope &parent_scope,
               const underworld::Source_Point &source_point) :
        name(name), signature(node, &return_type),
        scope(parent_scope, *this), block(scope),
        node(*this, return_type), source_point(source_point) {}

      Function(const std::string &name, Scope &parent_scope,
               const underworld::Source_Point &source_point) :
        name(name), signature(node, nullptr),
        scope(parent_scope, *this), block(scope),
        node(*this, Profession_Library::get_void()), source_point(source_point) {}

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
        return signature.get_parameters();
      }

      void add_parameter(Parameter &parameter) {
        signature.add_parameter(parameter);
      }

      void add_parameter(Parameter *parameter) {
        scope.add_minion(parameter);
        signature.add_parameter(*parameter);
      }

//      Minion &create_parameter(const std::string &name, Profession &profession);

      Profession_Node<Function> &get_node() override {
        return node;
      }

      Profession &get_profession() override {
        return *signature.get_return_type();
      }

      const Profession &get_profession() const override {
        return *signature.get_return_type();
      }

      void set_profession(Profession &value) override {
        signature.set_return_type(value);
      }

      const underworld::Source_Point &get_source_point()const override {
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

      Function_Signature &get_signature() {
        return signature;
      }

      const Function_Signature &get_function_signature() const {
        return signature;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}