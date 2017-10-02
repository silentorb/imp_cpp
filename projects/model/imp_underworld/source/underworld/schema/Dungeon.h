#pragma once

#include "Scope.h"
#include "Enchantment.h"

namespace underworld {

  class Dungeon : public Scope, public virtual Profession {
      const std::string name;
      const source_mapping::Source_Range source_point;
      bool _is_external = false;
      std::vector<Profession_Owner> contracts;
      std::vector<std::string> generic_parameters;
      Enchantment_Array enchantments;

  protected:
      void throw_already_exists(const std::string &member_name) const override;

  public:
      Dungeon(const std::string &name, Scope *parent) :
        name(name), Scope(parent) {}

      Dungeon(const std::string &name, Scope *parent, const source_mapping::Source_Range &source_point) :
        name(name), Scope(parent), source_point(source_point) {}

      virtual ~Dungeon();

      const std::string get_name() const {
        return name;
      }

      Profession_Type get_type() const override {
        return Profession_Type::dungeon;
      }

      Dungeon &get_dungeon() override {
        return *this;
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }

      void add_contract(Profession_Owner profession) {
        contracts.push_back(std::move(profession));
      }

      const std::vector<Profession_Owner> &get_contracts() const {
        return contracts;
      }

      void add_generic_parameter(const std::string &value) {
        generic_parameters.push_back(value);
      }

      const std::vector<std::string> &get_generic_parameters() const {
        return generic_parameters;
      }

      void add_enchantment(Enchantment_Owner &enchantment) {
        enchantments.push_back(enchantment);
      }

      const Enchantment_Array &get_enchantments() const {
        return enchantments;
      }
  };

  using Dungeon_Pointer = std::unique_ptr<Dungeon>;
}