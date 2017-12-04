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
//        throw std::runtime_error("Not supported.");
        return &get_member_node(member);
      }

      const Node *get_node() const override {
//        return &get_member_node(member);
        return &get_member_node(const_cast<Member&>(member));
//        throw std::runtime_error("Not supported.");
      }

      const std::string get_name() const override {
        return "..." + get_member_name(member);
      }

      Profession_Reference &get_profession() override {
        return get_member_profession_reference(member);
      }

      const Profession_Reference &get_profession() const override {
        return get_member_profession(member);
      }
  };

//
//  class Member_Expression_With_Node : public Common_Node_Expression {
//      Member member;
//
//  public:
//      Member_Expression_With_Node(Member member, Parent parent, Profession_Reference profession,
//                                  const source_mapping::Source_Range &source_range) :
//        Common_Node_Expression(parent, profession, source_range),
//        member(member) {}
//
//      virtual ~Member_Expression_With_Node() = default;
//
//      Expression_Type get_type() const override {
//        return Expression_Type::member;
//      }
//
//      const Member &get_member() const {
//        return member;
//      }
//
//      Member &get_member() {
//        return member;
//      }
//
//      void set_member(Member &value) {
//        member = value;
//      }
//
//      const std::string get_name() const override {
//        return "..." + get_member_name(member);
//      }
//
//      Profession_Reference &get_profession() override {
//        return get_member_profession_reference(member);
//      }
//
//      const Profession_Reference &get_profession() const override {
//        return get_member_profession(member);
//      }
//  };
}