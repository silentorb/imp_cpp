#pragma once

#include <memory>
#include <overworld/expressions/Block_Expression.h>
#include "Parameter.h"
#include "Function_Signature.h"
#include "Temporary_Interface.h"
#include "Parent.h"
#include "Scope.h"

namespace overworld {

  class Profession_Library;

  class Function {
  protected:
      Profession_Reference signature_reference;
      Function_Signature &signature;
      Common_Element element;
      Function *original = nullptr;
      virtual bool returns_a_value() const;
      std::vector<std::unique_ptr<Generic_Parameter>> owned_generic_parameters;
      Generic_Argument_Array arguments;
      Generic_Parameter_Array generic_parameters;
      Enchantment_Container enchantments;
      File *header_file = nullptr;
      std::vector<std::unique_ptr<Function>> variants;

  public:
      Function(const std::string &name, Parent parent, const source_mapping::Source_Range &source_point) :
        element(Element_Type::other, name, parent, source_point),
        signature_reference(new Function_Signature()),
        signature(*static_cast<Function_Signature *>(signature_reference.get())) {}

      Function(Function &original, std::vector<Profession_Reference> &professions) :
        Function(original.get_name(), original.get_element().get_parent(), source_mapping::Source_Range()) {
        this->original = &original;
        for (auto i = 0; i < generic_parameters.size(); ++i) {
          arguments.push_back(Generic_Argument_Owner(new Generic_Argument(*generic_parameters[i], professions[i])));
        }
      }

      virtual ~Function() {}

      const std::string get_name() const {
        return element.get_name();
      }

      bool is_constructor(const Parent &parent) const;

      virtual bool is_inline() const {
        return false;
      }

      Profession_Reference &get_profession_reference() {
        return signature_reference;
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
        throw std::runtime_error("Doesn't work the same way anymore: node.get_dungeon()");
//        auto &result = add_generic_parameter_to_vector(owned_generic_parameters, node.get_dungeon(), this);
//        generic_parameters.push_back(&result.get_owner());
//        return result.get_reference();
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

      const std::vector<Generic_Parameter *> &get_generic_parameters() const {
        return generic_parameters;
      }

      bool has_generic_arguments() const;
//      virtual Scope &get_parent_scope() = 0;
//      virtual const Scope &get_parent_scope() const = 0;


      Enchantment_Container &get_enchantments() {
        return enchantments;
      }

      const Enchantment_Container &get_enchantments() const {
        return enchantments;
      }

      bool has_enchantment(const Dungeon &enchantment) const {
        return enchantments.has_enchantment(enchantment);
      }

      Generic_Argument_Array &get_arguments() {
        return arguments;
      }

      const Generic_Argument_Array &get_arguments() const {
        return arguments;
      }

      const File *get_file() const {
        return header_file;
      }

      void set_file(File *value) {
        header_file = value;
      }

      const Common_Element &get_element() const {
        return element;
      }

      Common_Element &get_element() {
        return element;
      }

      Function &get_or_create_variant(Dungeon &containing_dungeon);
  };

  using Function_Owner = std::unique_ptr<Function>;

  class Virtual_Function : public Function {
//      Scope &parent_scope;

  public:
      Virtual_Function(const std::string &name, Dungeon &dungeon,
                       const source_mapping::Source_Range &source_point) :
        Function(name, dungeon, source_point) {}

      Virtual_Function(Function &original, std::vector<Profession_Reference> &professions) :
        Function(original, professions) {

      }

      virtual ~Virtual_Function() {}

//      Scope &get_parent_scope() override {
//        return parent_scope;
//      }

//      const Scope &get_parent_scope() const override {
//        return parent_scope;
//      }
  };

  class Function_With_Block : public Function {
      Scope scope;
      Block block;

  protected:
      bool returns_a_value() const override;

  public:
      Function_With_Block(const std::string &name, Scope &parent_scope, Parent parent,
                          const source_mapping::Source_Range &source_point) :
        Function(name, parent, source_point), scope(&parent_scope, Parent(*this)), block(scope) {}

      virtual ~Function_With_Block() {}

      Block &get_block() {
        return block;
      }

      const Block &get_block() const {
        return block;
      }

      const Scope &get_scope() const {
        return scope;
      }

      Scope &get_scope() {
        return scope;
      }

//      Scope &get_parent_scope() override {
//        return *scope.get_parent_scope();
//      }

//      const Scope &get_parent_scope() const override {
//        return *scope.get_parent_scope();
//      }

      bool is_inline() const override;
//      Temporary_Interface &get_or_create_interface(Parameter &parameter);
  };

  using Function_With_Block_Owner = std::unique_ptr<Function_With_Block>;

}