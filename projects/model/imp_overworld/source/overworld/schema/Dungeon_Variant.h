#pragma once

#include <overworld/schema/Dungeon.h>
#include "Variant.h"

namespace overworld {

  class Dungeon_Variant : public Dungeon_Interface {
      Variant variant;
      Dungeon &original;

  public:
      Dungeon_Variant(Dungeon &original, std::vector<Profession_Reference> &professions) :
        variant(original.get_name(), original.get_generic_parameters(), professions),
        original(original) {
      }

      virtual ~Dungeon_Variant() {
      }

      Profession_Type get_type() const  {
        return Profession_Type::variant;
      }

//      Scope *get_scope()  {
//        return original.get_scope();
//      }
//
//      const Scope *get_scope() const  {
//        return original.get_scope();
//      }

      File *get_file() const  {
        return original.get_file();
      }

      Node &get_node()  {
        throw std::runtime_error("Not supported.");
      }

      const std::string get_name() const  {
        return variant.get_name();
      }

      Ownership get_ownership() const override {
        return original.get_ownership();
      }

      Profession_Reference &get_base(Profession_Reference &self)  {
        return self;
      }

      Dungeon &get_original() override {
        return original;
      }

      const Dungeon &get_original() const override {
        return original;
      }

      Dungeon_Type get_dungeon_type() const override {
        return Dungeon_Type::variant;
      }

      const Generic_Parameter_Array &get_generic_parameters() const {
        return variant.get_generic_parameters();
      }

      Generic_Argument_Array &get_arguments() {
        return variant.get_arguments();
      }

      const Generic_Argument_Array &get_arguments() const {
        return variant.get_arguments();
      }

  };

  using Dungeon_Variant_Owner = std::unique_ptr<Dungeon_Variant>;
}