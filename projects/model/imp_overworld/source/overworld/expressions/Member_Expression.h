#pragma once

#include <overworld/schema/Member.h>
#include "Expression.h"

namespace overworld {

  class Member_Expression : public Common_Expression {
      Member *member;
      Member_Pointer owned_member; // Used to optionally own its member

  public:
      Member_Expression(Member &member) : member(&member) {}

      Member_Expression(Member_Pointer &minion) :
        owned_member(std::move(minion)) {
        member = owned_member.get();
      }

      virtual ~Member_Expression() {

      }

      Type get_type() const override {
        return Type::member;
      }

      Member &get_member() const {
        return *this->member;
      }

      Node *get_node() override {
        return &member->get_node();
      }
  };
}