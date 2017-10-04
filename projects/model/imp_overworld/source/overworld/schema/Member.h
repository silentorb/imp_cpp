#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  enum class Member_Type {
      array,
      dungeon,
      function,
      minion,
      profession,
  };

  class Member {
  public:

      virtual Member_Type get_member_type() const = 0;
      virtual Node &get_node() = 0;
      virtual const std::string get_name() const = 0;

      virtual ~Member() {}
  };

  template<typename T>
  class Generic_Member_Owner : public Member {
      std::unique_ptr<T> value;

  public:
      explicit Generic_Member_Owner(std::unique_ptr<T> parameter) :
        value(std::move(parameter)) {}

      Node &get_node() override {
        return value->get_node();
      }

      const std::string get_name() const override {
        return value->get_name();
      }
  };

  template<typename T>
  class Generic_Member_Reference : public Member {
  protected:
      T &value;

  public:
      explicit Generic_Member_Reference(T &value) :
        value(value) {}

      Node &get_node() override {
        return value.get_node();
      }

      const std::string get_name() const override {
        return value.get_name();
      }
  };

  using Member_Owner = std::unique_ptr<Member>;

  class Member_Reference_Array : public Member {
      std::vector<Member_Owner> values;

  public:
      Member_Type get_member_type() const override {
        return Member_Type::array;
      }

      Node &get_node() override {
        throw std::runtime_error("Not supported.");
      }

      const std::string get_name() const override {
        throw std::runtime_error("Not supported.");
      }

      void add_member(Member_Owner member) {
        values.push_back(std::move(member));
      }

      const std::vector<Member_Owner> &get_members() const {
        return values;
      }

      std::vector<Member_Owner> &get_members() {
        return values;
      }
  };

}