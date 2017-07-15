#pragma once

#include "Expression.h"
#include <map>

namespace underworld {

  class Instantiation : public Common_Expression {
      Profession_Owner profession;
      std::map<std::string, Expression_Owner> dictionary;
      const Source_Point source_point;

  public:
      Instantiation(Profession_Owner &profession, const Source_Point &source_point) :
        profession(std::move(profession)), source_point(source_point) {}

      virtual ~Instantiation() {}

      Type get_type() const override {
        return Type::instantiation;
      }

      void add_expression(const std::string &key, Expression_Owner value) {
        dictionary.insert(std::make_pair(key, std::move(value)));
      }

      const std::map<std::string, Expression_Owner> &get_dictionary() const {
        return dictionary;
      }

      const Profession_Owner &get_profession() const {
        return profession;
      }

      const Source_Point &get_source_point() const {
        return source_point;
      }
  };
}