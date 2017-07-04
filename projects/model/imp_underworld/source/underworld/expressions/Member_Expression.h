#pragma once

#include <underworld/schema/member.h>
#include "Expression.h"

namespace underworld {

  class Member_Expression : public virtual Common_Expression {
      const std:: string name;

  public:
      Member_Expression(const std::string &name) : name(name) {}

      virtual ~Member_Expression() {

      }

      virtual Type get_type() const override {
        return Type::member;
      }


      const std::string get_name() const override {
        return name;
      }

  };

}