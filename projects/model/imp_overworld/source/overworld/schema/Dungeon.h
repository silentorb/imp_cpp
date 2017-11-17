#pragma once

#include "Scope.h"
#include "File.h"
#include "Generic_Parameter.h"
#include "Dungeon.h"
#include "Enchantment.h"
#include "Enchantment_Container.h"
#include "Generic_Argument.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon {
      std::string name;
      Scope scope;
      Generic_Parameter_Array generic_parameters;
      Generic_Argument_Array arguments;
      Dungeon *original;
      File *header_file = nullptr;
      Ownership default_ownership = Ownership::unknown;
      Dungeon *base_dungeon = nullptr;
      std::vector<Profession_Reference> contracts;
      std::vector<std::unique_ptr<Generic_Parameter>> owned_generic_parameters;
      Enchantment_Container enchantments;
      Profession_Reference self;

      const std::string format_debug_name() const;

  public:
      Dungeon(const std::string &name, Scope &parent, const source_mapping::Source_Range source_range);

      Dungeon(const std::string &name, Scope &parent) :
        Dungeon(name, parent, source_mapping::Source_Range()) {}

      Dungeon(Dungeon &original, std::vector<Profession_Reference> &professions) :
        Dungeon(original.get_name(), *original.get_scope().get_parent_scope(), source_mapping::Source_Range()) {
        this->original = &original;
        for (auto i = 0; i < generic_parameters.size(); ++i) {
          arguments.push_back(Generic_Argument_Owner(new Generic_Argument(*generic_parameters[i], professions[i])));
        }
      }

      explicit Dungeon(const std::string &name);

      Dungeon(const Dungeon &) = delete;

      virtual ~Dungeon() {}

      const std::string get_name() const {
        return name;
      }

      Profession_Reference &get_reference() {
        return self;
      }

      const Profession_Reference &get_profession() const {
        return self;
      }

      Dungeon *get_original2() {
        return original;
      }

      const Dungeon *get_original2() const {
        return original;
      }

      bool is_class() const;

      void set_file(File *value) {
        header_file = value;
      }

      void set_file(File &value) {
        header_file = &value;
      }

      Ownership get_ownership() const {
        return default_ownership;
      }

      Scope &get_scope() {
        return scope;
      }

      const std::vector<Minion_Owner> &get_minions() const {
        return scope.get_minions();
      }

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return scope.get_functions();
      }

      const Scope &get_scope() const {
        return scope;
      }

      Generic_Argument_Array &get_arguments() {
        return arguments;
      }

      const Generic_Argument_Array &get_arguments() const {
        return arguments;
      }

      File *get_file() const {
        return header_file;
      }

      Function &get_or_create_constructor();

      bool is_generic() const {
        return generic_parameters.size() > 0;
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

      bool is_external() const;

      Dungeon &add_dungeon(std::unique_ptr<Dungeon> dungeon);

      void set_default_ownership(Ownership value) {
        this->default_ownership = value;
      }

      Dungeon *get_base_dungeon() {
        return base_dungeon;
      }

      const Dungeon *get_base_dungeon() const {
        return base_dungeon;
      }

      bool is_interface() const {
        return false;
      }

      void add_contract(Dungeon &dungeon, Profession_Reference &profession) {
        if (!dungeon.is_interface()) {
          if (base_dungeon)
            throw std::runtime_error(get_name() + " already has a base dungeon.");

          base_dungeon = &dungeon;
        }

        contracts.push_back(profession);
      }

      const std::vector<Profession_Reference> *get_contracts() const {
        return &contracts;
      }

      std::vector<Profession_Reference> *get_contracts() {
        return &contracts;
      }

      Function &create_function(const std::string &name, Profession &profession,
                                const source_mapping::Source_Range &source_point = source_mapping::Source_Range());

      Minion &get_minion(const std::string &name);
      Member &get_member(const std::string &name);

      void add_generic_parameter(Generic_Parameter_Owner parameter) {
        generic_parameters.push_back(parameter.get());
        if (parameter->get_name() == "")
          parameter->set_name(get_generic_parameter_name(generic_parameters.size() - 1));

        scope.add_member(parameter->get_name(), Member(*parameter));
        owned_generic_parameters.push_back(std::move(parameter));
//        rename_generic_parameters(owned_generic_parameters);

      }

      const std::string get_debug_name() const {
        return format_debug_name();
      }

      std::vector<Generic_Parameter *> &get_generic_parameters() {
        return generic_parameters;
      }

      const std::vector<Generic_Parameter *> &get_generic_parameters() const {
        return generic_parameters;
      }

      bool has_generic_parameter(Generic_Parameter &parameter) {
        for (auto &item : generic_parameters) {
          if (item == &parameter)
            return true;
        }
        return false;
      }

//      Dungeon &get_original() override {
//        return *this;
//      }
//
//      const Dungeon &get_original() const override {
//        return *this;
//      }
  };

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
}