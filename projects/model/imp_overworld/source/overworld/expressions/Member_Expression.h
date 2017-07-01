#pragma once

#include "Expression.h"

namespace overworld {

  class Member_Expression : public Common_Expression {
      Member &member;

  public:
      Member_Expression(Member &minion) : member(minion) {}

      virtual ~Member_Expression() {

      }

      Type get_type() const override {
        return Type::minion;
      }

      Member &get_member() const {
        return this->member;
      }

      Node *get_node() override {
        return &member.get_node();
      }
  };
}