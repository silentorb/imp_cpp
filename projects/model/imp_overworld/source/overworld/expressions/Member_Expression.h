#pragma once

#include <overworld/schema/Member.h>
#include "Expression.h"

namespace overworld {

  class Member_Expression : public Common_Expression {
      Member member;

  public:
      Member_Expression(Member member, const source_mapping::Source_Range &source_range) :
        Common_Expression(source_range),
        member(member) {}

//      Member_Expression(Member &minion, const source_mapping::Source_Range &source_range) :
//        Common_Expression(source_range),
//        owned_member(std::move(minion)) {
//        member = owned_member.get();
//      }

      virtual ~Member_Expression() = default;

      Expression_Type get_type() const override {
        return Expression_Type::member;
      }

      const Member &get_member() const {
        return member;
      }

      Member &get_member() {
        return member;
      }

      void set_member(Member &value) {
        member = value;
      }

      Node *get_node() override {
        return &get_member_node(member);
      }

      const Node *get_node() const override {
        return &get_member_node(member);
      }

      const std::string get_name() const override {
        return "Member_Expression";
      }

      Profession_Reference &get_profession() override {
        throw std::runtime_error("Not supported.");
      }

      const Profession_Reference &get_profession() const override {
        throw std::runtime_error("Not supported.");
      }
  };
}