#pragma once

#include <overworld/imp_graph/Node.h>
#include <overworld/expressions/Expression.h>
#include <overworld/schema/professions/Profession_Library.h>
#include "Member.h"

//namespace overworld {

//  class Unresolved_Member : public virtual Member, public virtual Profession_Reference {
//      Profession_Node <Unresolved_Member> node;
//      Expression &first;
//      Expression *second = nullptr;  // Needs to be assigned after construction
//      const std::string member_name;
//      const source_mapping::Source_Range source_point;
//
//  public:
//      Unresolved_Member(Expression &first, const std::string &member_name,
//                        const source_mapping::Source_Range &source_point) :
//        first(first), member_name(member_name), node(*this,), source_point(source_point) {}
//
//      virtual ~Unresolved_Member() {}
//
//      Member_Type get_member_type() const override {
//        return Member_Type::unresolved;
//      }
//
//      void set_second(Expression &value) {
//        second = &value;
//      }
//
//      Node &get_node() override {
//        return node;
//      }
//
//      const std::string get_name() const override {
//        return member_name;
//      }
//
//      const Profession &get_profession() const override {
//        return Profession_Library::get_unknown();
//      }
//
//      Profession &get_profession() override {
//        return Profession_Library::get_unknown();
//      }
//
//      void set_profession(Profession &value) override {
//        throw std::runtime_error("Not yet implemented.");
//      }
//
//      const source_mapping::Source_Range &get_source_point() override {
//        return source_point;
//      }
//  };
//}