#pragma once

#include <underworld/schema/member.h>
#include "Expression.h"

namespace underworld {

  class Member_Expression : public virtual Common_Expression {
      Member &member;

  public:
      Member_Expression(Member &member) : member(member) {}

      virtual ~Member_Expression() {

      }

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

      const Profession &get_profession() const override {
        return member.get_profession();
      }
  };

  class Unresolved_Member_Expression : public virtual Common_Expression {
      std::string member_name;
      Source_Point source_point;

  public:
      Unresolved_Member_Expression(const std::string &member_name, const Source_Point &source_point) :
        member_name(member_name), source_point(source_point) {}

      virtual ~Unresolved_Member_Expression() {

      }

      virtual Type get_type() const override {
        return Type::unresolved_member;
      }

      const std::string &get_member_name() const {
        return member_name;
      }

      const Profession &get_profession() const override {
        return Profession_Library::get_primitive(Primitive_Type::Void);
      }

      const Source_Point &get_source_point() const {
        return source_point;
      }
  };
}