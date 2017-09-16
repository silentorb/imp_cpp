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

  class Function
    : public virtual Profession_Reference, public virtual Function_Interface {
  protected:
      Function_Signature signature;
      Profession_Node<Function> node;
      const std::string name;
      virtual bool returns_a_value() const;
      const source_mapping::Source_Point source_point;
      std::vector<Generic_Parameter_Owner> owned_generic_parameters;
      Generic_Parameter_Array generic_parameters;
      Enchantment_Container enchantments;

  public:
      Function(const std::string &name, Profession &return_type, Scope &parent_scope,
               Dungeon_Interface &dungeon,
               const source_mapping::Source_Point &source_point) :
        name(name), signature(node, &return_type),
        node(*this, return_type, &dungeon, this), source_point(source_point) {}

      Function(const std::string &name, Scope &parent_scope, Dungeon_Interface &dungeon,
               const source_mapping::Source_Point &source_point) :
        name(name), signature(node, nullptr),
        node(*this, Profession_Library::get_void(), &dungeon, this), source_point(source_point) {}

      virtual ~Function() {}

      const std::string get_name() const override {
        return name;
      }

      bool is_constructor() const;

      virtual bool is_inline() const {
        return false;
      }

      const std::vector<Parameter *> &get_parameters() const {
        return signature.get_parameters();
      }

      void add_parameter_to_signature(Parameter &parameter) {
        signature.add_parameter(parameter);
      }

      virtual void add_parameter(std::unique_ptr<Parameter> parameter) = 0;

//      Minion &create_parameter(const std::string &name, Profession &profession);

      Profession_Node<Function> &get_node() {
        return node;
      }

      Profession &get_profession() override {
        auto profession = signature.get_return_type();
        if (!profession)
          return Profession_Library::get_void();

        return *profession;
      }

      const Profession &get_profession() const override {
        auto profession = signature.get_return_type();
        if (!profession)
          return Profession_Library::get_void();

        return *profession;
      }

      void set_profession(Profession &value) override {
        signature.set_return_type(value);
      }

      const source_mapping::Source_Point &get_source_point() const override {
        return source_point;
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

      virtual Scope &get_parent_scope() = 0;
      virtual const Scope &get_parent_scope() const = 0;

      Function &get_original() override {
        return *this;
      }

      void add_enchantment(Enchantment &enchantment) {
        enchantments.add_enchantment(enchantment);
      }

      bool has_enchantment(const Enchantment &enchantment) const {
        return enchantments.has_enchantment(enchantment);
      }
  };

  using Function_Owner = std::unique_ptr<Function>;

  class Virtual_Function : public Function {
      std::vector<std::unique_ptr<Parameter>> parameters;
      Scope &parent_scope;

  public:
      Virtual_Function(const std::string &name, Profession &return_type, Scope &parent_scope,
                       Dungeon_Interface &dungeon, const source_mapping::Source_Point &source_point) :
        Function(name, return_type, parent_scope, dungeon, source_point), parent_scope(parent_scope) {}

      Virtual_Function(const std::string &name, Scope &parent_scope, Dungeon_Interface &dungeon,
                       const source_mapping::Source_Point &source_point) :
        Function(name, parent_scope, dungeon, source_point), parent_scope(parent_scope) {}

      virtual ~Virtual_Function() {}

      void add_parameter(std::unique_ptr<Parameter> parameter) override {
        Function::add_parameter_to_signature(*parameter);
        parameters.push_back(std::move(parameter));
      }

      Scope &get_parent_scope() override {
        return parent_scope;
      }

      const Scope &get_parent_scope() const override {
        return parent_scope;
      }
  };

  class Function_With_Block : public Function {
      Function_Scope scope;
      Block block;

  protected:
      bool returns_a_value() const override;

  public:
      Function_With_Block(const std::string &name, Profession &return_type, Scope &parent_scope,
                          Dungeon_Interface &dungeon, const source_mapping::Source_Point &source_point)
        : Function(name, return_type, parent_scope, dungeon, source_point), scope(parent_scope, *this), block(scope) {}

      Function_With_Block(const std::string &name, Scope &parent_scope, Dungeon_Interface &dungeon,
                          const source_mapping::Source_Point &source_point) :
        Function(name, parent_scope, dungeon, source_point), scope(parent_scope, *this), block(scope) {}

      virtual ~Function_With_Block() {}

      Block &get_block() {
        return block;
      }

      const Block &get_block() const {
        return block;
      }

      const Function_Scope &get_scope() const {
        return scope;
      }

      Function_Scope &get_scope() {
        return scope;
      }

      Scope &get_parent_scope() override {
        return *scope.get_parent();
      }

      const Scope &get_parent_scope() const override {
        return *scope.get_parent();
      }

      bool is_inline() const override;

      void add_parameter(std::unique_ptr<Parameter> parameter) override {
        signature.add_parameter(*parameter);
        scope.add_minion(std::move(parameter));
      }
  };

  class Member_Function : public Generic_Member_Reference<Function> {
  public:
      Member_Function(Function &value) : Generic_Member_Reference(value) {}

      Member_Type get_member_type() const override {
        return Member_Type::function;
      }

      Function &get_function() {
        return value;
      }
  };
}