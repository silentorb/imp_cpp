#pragma once

#include <overworld/schema/Member.h>
#include "Expression.h"

namespace overworld {

  class Member_Expression : public Common_Expression {
      Member *member;
      Member_Owner owned_member; // Used to optionally own its member

  public:
      Member_Expression(Member &member) : member(&member) {}

      Member_Expression(Member_Owner &minion) :
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

      void set_member(Member &value) {
        if (owned_member)
          throw std::runtime_error("Member_Expression already has an owned member and cannot change to a reference.");

        member = &value;
      }

      Node *get_node() override {
        return &member->get_node();
      }
  };
}