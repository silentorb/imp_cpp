#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "professions.h"
#include <underworld/schema/Function.h>
#include <overworld/expressions/Expression.h>

namespace overworld {

  class Function;

  class Scope {
      const underworld::Scope &source;

  protected:
      std::map<std::string, Member_Pointer> members;

//      void check_has_member(const std::string &member_name) {
//        if (members.count(member_name) != 0)
//          throw std::runtime_error(member_name + " already exists");
//      }

  public:
      Scope(const underworld::Scope &source);
      Function &create_function(const underworld::Function &input);
      Minion &create_minion(const underworld::Minion &input, const Profession &profession);

      std::map<std::string, Member_Pointer> &get_members() {
        return members;
      }

      const std::map<std::string, Member_Pointer> &get_members() const {
        return members;
      }

      Member &get_member(const std::string &name) {
        return *(members[name]);
      }

      Minion &get_minion(const std::string &name) {
        return *dynamic_cast<overworld::Minion *>(members[name].get());
      }
  };
}