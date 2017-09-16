#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  enum class Member_Type {
      dungeon,
      function,
      generic_parameter,
      variable,
  };

  class Member {
  public:

      virtual Member_Type get_member_type() const = 0;
      virtual Node &get_node() = 0;
      virtual const std::string get_name() const = 0;

      virtual ~Member() {}
  };

 template <typename T>
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

  template <typename T>
  class Generic_Member_Reference : public Member {
      T& value;

  public:
      explicit Generic_Member_Reference(T& value) :
        value(value) {}

      Node &get_node() override {
        return value.get_node();
      }

      const std::string get_name() const override {
        return value.get_name();
      }
  };

  using Member_Owner = std::unique_ptr<Member>;
}