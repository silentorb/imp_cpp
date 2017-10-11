#pragma once

#include "Expression.h"
#include "../schema/professions/Profession.h"
#include <map>
#include <source_mapping/Source_Point.h>

namespace overworld {

  class Instantiation : public Common_Expression {
      Profession *profession;
      std::map<Minion *, Expression_Owner> dictionary;
      Element_Reference_Node node;
      std::unique_ptr<Dungeon_Variant> dungeon_variant;

  public:
      Instantiation(Profession &profession, Dungeon_Interface *dungeon, Function_Interface *function,
                    const source_mapping::Source_Range &source_range) :
        Common_Expression(source_range),
        profession(&profession),
        node(element, dungeon, function) {}

      virtual ~Instantiation() = default;

      Expression_Type get_type() const override {
        return Expression_Type::instantiation;
      }

      const Dungeon_Variant &get_dungeon_variant() const {
        return *dungeon_variant;
      }

      void set_dungeon_variant(std::unique_ptr<Dungeon_Variant> value) {
        dungeon_variant = std::move(value);
      }

      void add_expression(Minion &minion, Expression_Owner value) {
        dictionary.insert(std::make_pair(&minion, std::move(value)));
      }

      const std::map<Minion *, Expression_Owner> &get_dictionary() const {
        return dictionary;
      }

      Profession &get_profession() override {
        return *profession;
      }

      Node *get_node() override {
        return &node;
      }

      const Node *get_node() const override {
        return &node;
      }

      const Profession &get_profession() const override {
        return *profession;
      }

      void set_profession(Profession &value) override {
        profession = &value;
      }

      const std::string get_name() const override {
        return "new " + profession->get_name();
      }
  };
}