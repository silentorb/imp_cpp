#pragma once

#include "Expression.h"
#include "../schema/professions/Profession.h"
#include <map>
#include <underworld/Source_Point.h>

namespace overworld {

  class Instantiation : public Common_Expression {
      Profession &profession;
      std::map<Minion *, Expression_Owner> dictionary;
      const underworld::Source_Point source_point;

  public:
      Instantiation(Profession &profession, const underworld::Source_Point &source_point) :
        profession(profession), source_point(source_point) {}

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

      Profession &get_profession() const {
        return profession;
      }

      const underworld::Source_Point &get_source_point() const {
        return source_point;
      }

      Node *get_node() override {
        return &const_cast<Profession *>(&profession)->get_node();
      }
  };
}