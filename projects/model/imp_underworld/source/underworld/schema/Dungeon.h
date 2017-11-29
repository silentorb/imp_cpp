#pragma once

#include "Scope.h"
#include "Enchantment.h"
#include "Generic_Parameter.h"

namespace underworld {

  class Dungeon : public Member {
      const std::string name;
      bool _is_external = false;
      std::vector<Profession_Owner> contracts;
      std::vector<Generic_Parameter> generic_parameters;
//      Enchantment_Array enchantments;
      Scope scope;

  protected:
//      void throw_already_exists(const std::string &member_name) const override;

  public:
//      Dungeon(const std::string &name, Scope *parent) :
//        name(name), scope(parent) {}

      Dungeon(const std::string &name, Scope *parent,
              const source_mapping::Source_Range &source_point = source_mapping::Source_Range()) :
        name(name), scope(parent), Member(source_point) {}

      virtual ~Dungeon();

      const std::string get_name() const {
        return name;
      }

      Type get_type() const override {
        return Type::dungeon;
      }

      const Scope &get_scope() const {
        return scope;
      }

      Scope &get_scope() {
        return scope;
      }

//      Profession_Type get_type() const override {
//        return Profession_Type::dungeon;
//      }

//      Dungeon &get_dungeon() {
//        return *this;
//      }

//      const source_mapping::Source_Range get_source_point() const {
//        return source_point;
//      }

      void add_contract(Profession_Owner profession) {
        contracts.push_back(std::move(profession));
      }

      const std::vector<Profession_Owner> &get_contracts() const {
        return contracts;
      }

      void add_generic_parameter(const Generic_Parameter &value) {
        generic_parameters.push_back(value);
      }

      const std::vector<Generic_Parameter> &get_generic_parameters() const {
        return generic_parameters;
      }

//      void add_enchantment(Enchantment_Owner &enchantment) {
//        enchantments.push_back(enchantment);
//      }
//
//      const Enchantment_Array &get_enchantments() const {
//        return enchantments;
//      }
  };

  using Dungeon_Pointer = std::unique_ptr<Dungeon>;
}