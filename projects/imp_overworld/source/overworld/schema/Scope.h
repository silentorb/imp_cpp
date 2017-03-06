#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "professions.h"

namespace overworld {

  class Function;

  class Scope {
  protected:
      std::map<std::string, Member_Pointer> members;

//      void check_has_member(const std::string &member_name) {
//        if (members.count(member_name) != 0)
//          throw std::runtime_error(member_name + " already exists");
//      }

  public:
      Function &create_function(const std::string &name);
      Minion &create_minion(const std::string &name, const Profession &profession);

      std::map<std::string, Member_Pointer> &get_members() {
        return members;
      }
  };
}