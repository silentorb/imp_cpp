#pragma once

#include <memory>
#include <overworld/expressions/Block_Expression.h>
#include <overworld/schema/professions/Profession_Library.h>
#include "Parameter.h"
#include "Function_Interface.h"

namespace overworld {

  class Profession_Library;

  class Function_Signature {
      std::vector<Parameter *> parameters;
      Node &node;
      Profession *return_type = nullptr;

  public:

      Function_Signature(Node &node, Profession *return_type = nullptr) :
        node(node), return_type(return_type) {}

      const std::vector<Parameter *> &get_parameters() const {
        return parameters;
      }

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

  class Function : public virtual Member, public virtual Profession_Reference, public virtual Function_Interface {
      Function_Scope scope;
      Block block;
      Function_Signature signature;
      Profession_Node<Function> node;
      const std::string name;
      bool _is_static = false;
      bool returns_a_value() const;
      const underworld::Source_Point source_point;
      std::vector<Generic_Parameter_Owner> owned_generic_parameters;
      Generic_Parameter_Array generic_parameters;

  public:
      Function(const std::string &name, Profession &return_type, Scope &parent_scope, Dungeon_Interface &dungeon,
               const underworld::Source_Point &source_point) :
        name(name), signature(node, &return_type),
        scope(parent_scope, *this), block(scope),
        node(*this, return_type, &dungeon, this), source_point(source_point) {}

      Function(const std::string &name, Scope &parent_scope, Dungeon_Interface &dungeon,
               const underworld::Source_Point &source_point) :
        name(name), signature(node, nullptr),
        scope(parent_scope, *this), block(scope),
        node(*this, Profession_Library::get_void(), &dungeon, this), source_point(source_point) {}

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

      bool is_inline() const;

      const std::vector<Parameter *> &get_parameters() const {
        return signature.get_parameters();
      }

      void add_parameter(Parameter &parameter) {
        signature.add_parameter(parameter);
      }

      void add_parameter(std::unique_ptr<Parameter> parameter) {
        signature.add_parameter(*parameter);
        scope.add_minion(std::move(parameter));
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

      const underworld::Source_Point &get_source_point() const override {
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

      Generic_Parameter &add_generic_parameter() {
        auto &result = add_generic_parameter_to_vector(owned_generic_parameters, node.get_dungeon(), this);
        generic_parameters.push_back(&result);
        return result;
      }

      Generic_Parameter_Owner detach_generic_parameter(Generic_Parameter &parameter) {
        for (auto i = generic_parameters.begin(); i != generic_parameters.end(); i++) {
          if ((*i) == &parameter) {
            generic_parameters.erase(i);
            break;
          }
        }

        for (auto i = owned_generic_parameters.begin(); i != owned_generic_parameters.end(); i++) {
          if ((*i).get() == &parameter) {
            auto result = std::move(*i);
            owned_generic_parameters.erase(i);
            return std::move(result);
          }
        }

        throw std::runtime_error("Could not find generic_parameter " + parameter.get_name() + ".");
      }

      std::vector<Generic_Parameter *> &get_generic_parameters() {
        return generic_parameters;
      }

      const std::vector<Generic_Parameter *> &get_generic_parameters() const override {
        return generic_parameters;
      }

      Function &get_original() override {
        return *this;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;
}