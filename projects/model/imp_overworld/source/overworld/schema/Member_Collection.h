#pragma once

#include <string>
#include <map>
#include "Member.h"

namespace overworld {

  using Members = std::vector<Member>;

  class Member_Collection {
      std::map<std::string, Member> members;
      std::map<std::string, Members> overloaded_members;

      void overload(const std::string &name, Member &member);
      void add_overload(const std::string &name, Member &member);

  public:
      void add_member(const std::string &name, Member member);
      Member *get_member_or_null(const std::string &name);
      Member &get_member(const std::string &name);

      std::map<std::string, Member> &get_members() {
        return members;
      }

  };
}