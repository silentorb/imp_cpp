#pragma once

#include <underworld/schema/Dungeon.h>
#include "Scope.h"
#include "File.h"
#include "Generic_Parameter.h"
#include "Dungeon_Interface.h"
#include "Member_Profession.h"
#include "Enchantment.h"
#include <vector>

namespace overworld {

  class Dungeon;

  using Dungeon_Owner = std::unique_ptr<Dungeon>;
  using Dungeons = std::vector<Dungeon_Owner>;

  class Dungeon : public Scope, public virtual Dungeon_Interface, public virtual Profession_Reference {
      File *header_file = nullptr;
      Profession_Node<Dungeon> node;
      const std::string name;
      const source_mapping::Source_Point source_point;
      Ownership default_ownership = Ownership::owner;
      Dungeon *base_dungeon = nullptr;
      std::vector<Profession *> contracts;
      std::vector<Generic_Parameter_Owner> owned_generic_parameters;
      Generic_Parameter_Array generic_parameters;
      Enchantment_Container enchantments;

  public:
      Dungeon(const std::string &name, Scope &parent) :
        name(name), Scope(&parent), node(*this, *this, this, nullptr) {}

      Dungeon(const std::string &name, Scope &parent, const source_mapping::Source_Point source_point) :
        name(name), Scope(&parent), node(*this, *this, this, nullptr), source_point(source_point) {}

      Dungeon(const std::string &name) :
        name(name), Scope(nullptr), node(*this, *this, this, nullptr) {}

      virtual ~Dungeon() {}

      const std::string get_name() const {
        return name;
      }

      virtual const std::string get_cpp_name() const {
        return name;
      }

      bool is_class() const;

      void set_file(File *value) {
        header_file = value;
      }

      void set_file(File &value) {
        header_file = &value;
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

      void set_profession(Profession &value) override {

      }

      Profession &get_profession() override {
        return *this;
      }

      const Profession &get_profession() const override {
        return *this;
      }

      Dungeon &get_dungeon() override {
        return *this;
      }

      const source_mapping::Source_Point &get_source_point() const override {
        return source_point;
      }

      Function &get_or_create_constructor();

      void add_enchantment(Enchantment &enchantment);

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
                                const source_mapping::Source_Point &source_point = source_mapping::Source_Point());

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

  class Cpp_Dungeon : public Dungeon {
      std::string cpp_name;

  public:
      Cpp_Dungeon(const std::string &name, const std::string &cpp_name, Scope &parent) :
        Dungeon(name, parent), cpp_name(cpp_name) {}

      virtual ~Cpp_Dungeon() {}

      const std::string get_cpp_name() const override {
        return cpp_name;
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