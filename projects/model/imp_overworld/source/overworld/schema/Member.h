#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Function;

  class Dungeon;

  class Minion;

  enum class Member_Type {
      none,
      dungeon,
      function,
      minion,
      profession,
  };

  class Member {
      Member_Type type;
      void *value;

  public:
      Member() : type(Member_Type::none), value(nullptr) {}

      Member(Dungeon &dungeon) :
        type(Member_Type::dungeon), value(&dungeon) {}

      Member(const Dungeon &dungeon) :
        type(Member_Type::dungeon), value(const_cast<Dungeon *>(&dungeon)) {}

      // Hack to differentiate from the Dungeon constructor.
      Member(Profession_Reference &profession, bool) :
        type(Member_Type::profession), value(&profession) {
        throw std::runtime_error("Not supported.");
      }

      Member(const Profession &profession, bool) :
        type(Member_Type::profession), value(const_cast<Profession *>(&profession)) {}

      Member(Function &function) :
        type(Member_Type::function), value(&function) {}

      Member(Minion &minion) :
        type(Member_Type::minion), value(&minion) {}

      Member_Type get_type() const {
        return type;
      }

      const Function &get_function() const {
        return *(Function *) value;
      }

      const Dungeon &get_dungeon() const {
        return *(Dungeon *) value;
      }

      const Minion &get_minion() const {
        return *(Minion *) value;
      }

      Function &get_function() {
        return *(Function *) value;
      }

      Dungeon &get_dungeon() {
        return *(Dungeon *) value;
      }

      Profession &get_profession() {
        return *(Profession *) value;
      }

      const Profession &get_profession() const {
        return *(Profession *) value;
      }

      Minion &get_minion() {
        return *(Minion *) value;
      }

      const void *get_value() const {
        return value;
      }
  };

  const Node &get_member_node(const Member &member);

  const std::string get_member_name(const Member &member);

  Node &get_member_node(Member &member);

  const std::string get_member_name(Member &member);

//  template<typename T>
//  class Generic_Member : public Member {
//      std::unique_ptr<T> value;
//
//  public:
//      explicit Generic_Member(std::unique_ptr<T> parameter) :
//        value(std::move(parameter)) {}
//
//      Node &get_node() override {
//        return value->get_node();
//      }
//
//      const std::string get_name() const override {
//        return value->get_name();
//      }
//  };
//
//  template<typename T>
//  class Generic_Member_Reference : public Member {
//  protected:
//      T &value;
//
//  public:
//      explicit Generic_Member_Reference(T &value) :
//        value(value) {}
//
//      Node &get_node() override {
//        return value.get_node();
//      }
//
//      const std::string get_name() const override {
//        return value.get_name();
//      }
//  };
//
//  using Member = std::unique_ptr<Member>;
//
//  class Member_Reference_Array : public Member {
//      std::vector<Member> values;
//
//  public:
//      Member_Type get_type() const override {
//        return Member_Type::array;
//      }
//
//      Node &get_node() override {
//        throw std::runtime_error("Not supported.");
//      }
//
//      const std::string get_name() const override {
//        throw std::runtime_error("Not supported.");
//      }
//
//      void add_member(Member member) {
//        values.push_back(std::move(member));
//      }
//
//      const std::vector<Member> &get_members() const {
//        return values;
//      }
//
//      std::vector<Member> &get_members() {
//        return values;
//      }
//  };

}