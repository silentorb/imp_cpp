#pragma once

#include <underworld/schema/member.h>
#include "Expression.h"

namespace underworld {

  class Member_Expression : public Common_Expression {
      const std::string name;
      source_mapping::Source_Range point;

  public:
      Member_Expression(const std::string &name, const source_mapping::Source_Range &point) :
        name(name), point(point) {}

      virtual ~Member_Expression() {

      }

      virtual Expression_Type get_type() const override {
        return Expression_Type::member;
      }

      const std::string get_name() const override {
        return name;
      }

      const source_mapping::Source_Range get_source_point() const override {
        return point;
      }
  };

}