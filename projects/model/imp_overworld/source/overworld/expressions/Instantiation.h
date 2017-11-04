#pragma once

#include "Expression.h"
#include "../schema/professions/Profession.h"
#include <map>
#include <source_mapping/Source_Point.h>

namespace overworld {

  class Instantiation : public Common_Node_Expression {
      std::map<Minion *, Expression_Owner> dictionary;

  public:
      Instantiation(Profession_Reference &profession, Parent parent,
                    const source_mapping::Source_Range &source_range) :
        Common_Node_Expression(parent, profession, source_range) {}

      virtual ~Instantiation() = default;

      Expression_Type get_type() const override {
        return Expression_Type::instantiation;
      }

//      const Dungeon_Variant &get_dungeon_variant() const {
//        return *dungeon_variant;
//      }
//
//      void set_dungeon_variant(std::unique_ptr<Dungeon_Variant> value) {
//        dungeon_variant = std::move(value);
//      }

      void add_expression(Minion &minion, Expression_Owner value) {
        dictionary.insert(std::make_pair(&minion, std::move(value)));
      }

      const std::map<Minion *, Expression_Owner> &get_dictionary() const {
        return dictionary;
      }

      Profession_Reference &get_profession() override {
        return node.get_profession();
      }

      const Profession_Reference &get_profession() const override {
        return node.get_profession();
      }

//      void set_profession(Profession_Reference &value) override {
//        node.set_profession(value);
//      }

      const std::string get_name() const override {
        return "new " + get_profession().get_name();
      }
  };
}