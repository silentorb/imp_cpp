#pragma once

#include "Expression.h"
#include <map>

namespace underworld {

  class Instantiation : public Common_Expression {
      Profession_Owner profession;
      std::map<std::string, Expression_Owner> dictionary;
      const source_mapping::Source_Range source_point;

  public:
      Instantiation(Profession_Owner profession, const source_mapping::Source_Range &source_point) :
        profession(std::move(profession)), source_point(source_point) {}

      virtual ~Instantiation() {}

      Expression_Type get_type() const override {
        return Expression_Type::instantiation;
      }

      void add_expression(const std::string &key, Expression_Owner value) {
        dictionary.insert(std::make_pair(key, std::move(value)));
      }

      const std::map<std::string, Expression_Owner> &get_dictionary() const {
        return dictionary;
      }

      const Profession &get_profession_expression() const {
        return *profession;
      }

      const source_mapping::Source_Range get_source_point() const override {
        return source_point;
      }

  };
}