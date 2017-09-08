#pragma once

#include "Expression.h"
#include "../schema/professions/Profession.h"
#include <map>
#include <source_mapping/Source_Point.h>

namespace overworld {

  class Instantiation : public Common_Expression, public virtual Profession_Reference {
      Profession *profession;
      std::map<Minion *, Expression_Owner> dictionary;
      const source_mapping::Source_Point source_point;
      Profession_Node <Instantiation> node;

  public:
      Instantiation(Profession &profession, const source_mapping::Source_Point &source_point, Dungeon_Interface *dungeon,
                    Function_Interface *function) :
        profession(&profession), source_point(source_point), node(*this, profession, dungeon, function) {}

      virtual ~Instantiation() {}

      Type get_type() const override {
        return Type::instantiation;
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

      const source_mapping::Source_Point &get_source_point() const {
        return source_point;
      }

      Node *get_node() override {
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

      Element_Type get_element_type() const override {
        return Element_Type::instantiation;
      }
  };
}