#pragma once

#include <memory>
#include <overworld/expressions/Block_Expression.h>
#include <overworld/schema/professions/Profession_Library.h>
#include "Parameter.h"
#include "Function_Interface.h"
#include "Function_Signature.h"

namespace overworld {

  class Profession_Library;

  class Function : public Function_Interface {
  protected:
      Function_Signature signature;
      Profession_Reference signature_reference;
      Common_Element element;
      Element_Reference_Node node;
      virtual bool returns_a_value() const;
      std::vector<Owned_Profession_Reference<Generic_Parameter>> owned_generic_parameters;
      Generic_Parameter_Array generic_parameters;
      Enchantment_Container enchantments;
      File *header_file = nullptr;

      const Profession &_get_profession() const {
        return signature;
//        auto profession = signature.get_return_type();
//        if (!profession)
//          return Profession_Library::get_unknown();
//
//        return *profession;
      }

      Profession_Reference &_get_profession() {
        return signature_reference;
      }

  public:
      Function(const std::string &name, Profession &return_type, Scope &parent_scope,
               Dungeon_Interface &dungeon,
               const source_mapping::Source_Range &source_point) :
        element(Element_Type::other, name, get_profession(), source_point),
        node(element, &dungeon, this),
        signature_reference(signature) {}

      Function(const std::string &name, Scope &parent_scope, Dungeon_Interface &dungeon,
               const source_mapping::Source_Range &source_point) :
        element(Element_Type::other, name, get_profession(), source_point),
        node(element, &dungeon, this) {}

      virtual ~Function() {}

      const std::string get_name() const {
        return element.get_name();
      }

      bool is_constructor() const;

      virtual bool is_inline() const {
        return false;
      }

      Element_Reference_Node &get_node() {
        return node;
      }

      Profession_Reference &get_profession() {
        return _get_profession();
      }

      const Profession &get_profession() const {
        return _get_profession();
      }

      void set_profession(Profession_Reference &value, Profession_Setter &setter) {
        signature.set_last_profession(value, setter);
      }

      void finalize(overworld::Profession_Library &profession_library);

      Function_Signature &get_signature() {
        return signature;
      }

      const Function_Signature &get_signature() const {
        return signature;
      }

      Profession_Reference &add_generic_parameter() {
        auto &result = add_generic_parameter_to_vector(owned_generic_parameters, node.get_dungeon(), this);
        generic_parameters.push_back(&result.get_owner());
        return result.get_reference();
      }

      Generic_Parameter_Owner detach_generic_parameter(Generic_Parameter &parameter) {
        for (auto i = generic_parameters.begin(); i != generic_parameters.end(); i++) {
          if ((*i) == &parameter) {
            generic_parameters.erase(i);
            break;
          }
        }

        for (auto i = owned_generic_parameters.begin(); i != owned_generic_parameters.end(); i++) {
          if (&(*i).get_owner() == &parameter) {
            throw std::runtime_error("No longer works like this...");
//            auto result = std::move(*i);
//            owned_generic_parameters.erase(i);
//            return std::move(result);
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

      Enchantment_Container &get_enchantments() {
        return enchantments;
      }

      const Enchantment_Container &get_enchantments() const {
        return enchantments;
      }

      bool has_enchantment(const Dungeon &enchantment) const {
        return enchantments.has_enchantment(enchantment);
      }

      const File *get_file() const {
        return header_file;
      }

      void set_file(File *value) {
        header_file = value;
      }
  };

  using Function_Owner = std::unique_ptr<Function>;

  class Virtual_Function : public Function {
      Scope &parent_scope;

  public:
      Virtual_Function(const std::string &name, Profession &return_type, Scope &parent_scope,
                       Dungeon_Interface &dungeon, const source_mapping::Source_Range &source_point) :
        Function(name, return_type, parent_scope, dungeon, source_point), parent_scope(parent_scope) {}

      Virtual_Function(const std::string &name, Scope &parent_scope, Dungeon_Interface &dungeon,
                       const source_mapping::Source_Range &source_point) :
        Function(name, parent_scope, dungeon, source_point), parent_scope(parent_scope) {}

      virtual ~Virtual_Function() {}

      Scope &get_parent_scope() override {
        return parent_scope;
      }

      const Scope &get_parent_scope() const override {
        return parent_scope;
      }
  };

  class Parameter_Temporary_Interface {
      Parameter &parameter;
      std::unique_ptr<Dungeon> interface;

  public:
      Parameter_Temporary_Interface(Parameter &parameter, std::unique_ptr<Dungeon> interface) :
        parameter(parameter), interface(std::move(interface)) {}

      Parameter &get_parameter() const {
        return parameter;
      }

      const std::unique_ptr<Dungeon> &get_interface() const {
        return interface;
      }
  };

  class Temporary_Interface_Manager {
      std::vector<Parameter_Temporary_Interface> entries;

  public:
      std::vector<Parameter_Temporary_Interface> &get_entries() {
        return entries;
      }

      void add(Parameter &parameter, Dungeon *interface) {
        entries.push_back(Parameter_Temporary_Interface(
          parameter, std::move(std::unique_ptr<Dungeon>(interface))
        ));
      }
  };

  class Function_With_Block : public Function {
      Function_Scope scope;
      Block block;
      std::unique_ptr<Temporary_Interface_Manager> temporary_interface_manager;

  protected:
      bool returns_a_value() const override;

  public:
      Function_With_Block(const std::string &name, Profession &return_type, Scope &parent_scope,
                          Dungeon_Interface &dungeon, const source_mapping::Source_Range &source_point)
        : Function(name, return_type, parent_scope, dungeon, source_point), scope(parent_scope, *this), block(scope) {}

      Function_With_Block(const std::string &name, Scope &parent_scope, Dungeon_Interface &dungeon,
                          const source_mapping::Source_Range &source_point) :
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
      Dungeon &get_or_create_interface(Parameter &parameter);
  };

  using Function_With_Block_Owner = std::unique_ptr<Function_With_Block>;

//  class Member_Function : public Generic_Member_Reference<Function> {
//  public:
//      Member_Function(Function &value) : Generic_Member_Reference(value) {}
//
//      Member_Type get_type() const override {
//        return Member_Type::function;
//      }
//
//      const Function &get_function() const {
//        return value;
//      }
//
//      Function &get_function() {
//        return value;
//      }
//  };
}