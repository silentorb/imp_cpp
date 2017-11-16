#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include "File.h"
#include "Generic_Parameter.h"
#include "Basic_Dungeon.h"
#include "Enchantment.h"
#include "Enchantment_Container.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Basic_Dungeon {
//      Common_Element element;
      std::string name;
      Scope scope;

      File *header_file = nullptr;
      Ownership default_ownership = Ownership::unknown;
      Dungeon *base_dungeon = nullptr;
      std::vector<Profession_Reference> contracts;
      std::vector<Owned_Profession_Reference<Generic_Parameter>> owned_generic_parameters;
      Generic_Parameter_Array generic_parameters;
      Enchantment_Container enchantments;
      Profession_Reference self;

      const std::string format_debug_name() const;

  public:

      Dungeon(const std::string &name, Scope &parent, const source_mapping::Source_Range source_point);

      Dungeon(const std::string &name, Scope &parent) :
        Dungeon(name, parent, source_mapping::Source_Range()) {}

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

      bool is_class() const;

      void set_file(File *value) {
        header_file = value;
      }

      void set_file(File &value) {
        header_file = &value;
      }

      Ownership get_ownership() const override {
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

//      void set_file(std::unique_ptr<File> value) {
//        header_file = value.get();
//        header_file_owner = std::move(value);
//      }

      File *get_file() const {
        return header_file;
      }

//      Dungeon &get_dungeon() override {
//        return *this;
//      }

      Function &get_or_create_constructor();

//      void add_enchantment(Enchantment &enchantment);

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

      Dungeon_Type get_dungeon_type() const override {
        return Dungeon_Type::original;
      }

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

//      Scope_Type get_scope_type() const override {
//        return Scope_Type::dungeon;
//      }

      Minion &get_minion(const std::string &name);
      Member &get_member(const std::string &name);

      void add_generic_parameter(Generic_Parameter_Owner parameter) {
        generic_parameters.push_back(parameter.get());
        if (parameter->get_name() == "")
          parameter->set_name(get_generic_parameter_name(generic_parameters.size() - 1));

        scope.add_member(parameter->get_name(), Member(parameter->get_reference()));
				auto temp = std::move(parameter);
				auto k = 0;
//        owned_generic_parameters.push_back(std::move(parameter));
//        rename_generic_parameters(owned_generic_parameters);

      }

      const std::string get_debug_name() const override {
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

      Dungeon &get_original() override {
        return *this;
      }

      const Dungeon &get_original() const override {
        return *this;
      }
  };

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
}