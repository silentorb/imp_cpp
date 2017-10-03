#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include "File.h"
#include "Generic_Parameter.h"
#include "Dungeon_Interface.h"
#include "Member_Profession.h"
#include "Enchantment.h"
#include "Enchantment_Container.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Scope, public virtual Dungeon_Interface {
      Common_Element element;
      File *header_file = nullptr;
      std::unique_ptr<File> header_file_owner;
      Element_Reference_Node node;
      Ownership default_ownership = Ownership::owner;
      Dungeon *base_dungeon = nullptr;
      std::vector<Profession *> contracts;
      std::vector<Generic_Parameter_Owner> owned_generic_parameters;
      Generic_Parameter_Array generic_parameters;
      Enchantment_Container enchantments;

      const std::string format_debug_name() const;

  public:
      Dungeon(const std::string &name, Scope &parent) :
        element(Element_Type::other, name, *this, source_mapping::Source_Range()),
        Scope(&parent), node(element, this, nullptr) {}

      Dungeon(const std::string &name, Scope &parent, const source_mapping::Source_Range source_point) :
        element(Element_Type::other, name, *this, source_point),
        Scope(&parent), node(element, this, nullptr) {}

      Dungeon(const std::string &name) :
        element(Element_Type::other, name, *this, source_mapping::Source_Range()),
        Scope(nullptr), node(element, this, nullptr) {}

      Dungeon(const Dungeon &) = delete;

      virtual ~Dungeon() {}

      const std::string get_name() const {
        return element.get_name();
      }

      bool is_class() const;

      void set_file(File *value) {
        header_file = value;
      }

      void set_file(File &value) {
        header_file = &value;
      }

      void set_file(std::unique_ptr<File> value) {
        header_file = value.get();
        header_file_owner = std::move(value);
      }

      File *get_file() const {
        return header_file;
      }

      Profession_Type get_type() const override {
        return Profession_Type::dungeon;
      }

      Scope *get_scope() override {
        return parent;
      }

      const Scope *get_scope() const override {
        return parent;
      }

      virtual Node &get_node() override {
        return node;
      }

      Dungeon &get_dungeon() override {
        return *this;
      }

      Function &get_or_create_constructor();

//      void add_enchantment(Enchantment &enchantment);

      Enchantment_Container &get_enchantments() {
        return enchantments;
      }

      const Enchantment_Container &get_enchantments() const {
        return enchantments;
      }

      bool has_enchantment(const Enchantment &enchantment) const {
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

      Ownership get_ownership() const override {
        return default_ownership;
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
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

      void add_contract(Dungeon &dungeon) {
        if (!dungeon.is_interface()) {
          if (base_dungeon)
            throw std::runtime_error(get_name() + " already has a base dungeon.");

          base_dungeon = &dungeon;
        }

        contracts.push_back(&dungeon);
      }

      const std::vector<Profession *> *get_contracts() const override {
        return &contracts;
      }

      std::vector<Profession *> *get_contracts() override {
        return &contracts;
      }

      Function &create_function(const std::string &name, Profession &profession,
                                const source_mapping::Source_Range &source_point = source_mapping::Source_Range());

      Scope_Type get_scope_type() const override {
        return Scope_Type::dungeon;
      }

      Minion &get_minion(const std::string &name) override;
      Member &get_member(const std::string &name) override;

      void add_generic_parameter(Generic_Parameter_Owner parameter) {
        generic_parameters.push_back(parameter.get());
        if (parameter->get_name() == "")
          parameter->set_name(get_generic_parameter_name(generic_parameters.size() - 1));

        add_member(parameter->get_name(), Member_Owner(new Member_Profession(*parameter)));
        owned_generic_parameters.push_back(std::move(parameter));
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

  class Member_Dungeon : public Generic_Member_Reference<Dungeon> {
  public:
      Member_Dungeon(Dungeon &value) : Generic_Member_Reference(value) {}

      Member_Type get_member_type() const override {
        return Member_Type::dungeon;
      }

      Dungeon &get_dungeon() {
        return value;
      }
  };
}