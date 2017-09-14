#pragma once

#include "Expression.h"
#include <map>

namespace underworld {

  class Instantiation : public Common_Expression {
      Expression_Owner profession;
      std::map<std::string, Expression_Owner> dictionary;
      const source_mapping::Source_Point source_point;

  public:
      Instantiation(Expression_Owner profession, const source_mapping::Source_Point &source_point) :
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

      const Expression &get_profession_expression() const {
        return *profession;
      }

      const source_mapping::Source_Point &get_source_point() const {
        return source_point;
      }

  };
}