#pragma once

#include <underworld/schema/member.h>
#include "Expression.h"

namespace underworld {

  class Member_Expression : public virtual Common_Expression {
      Member &member;

  public:
      Member_Expression(Member &member) : member(member) {}

      virtual Type get_type() const override {
        return Type::member;
      }

      const Member &get_member() const {
        return member;
      }

      Member &get_member() {
        return member;
      }

      const std::string get_name() const override {
        return member.get_name();
      }
  };
}