#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "professions.h"

namespace underworld {

  class Function;

  class Scope {
  protected:
      std::map<std::string, Member_Pointer> members;

      void check_has_member(const std::string &member_name) {
        if (members.count(member_name) != 0)
          throw_already_exists(member_name);
      }

      virtual void throw_already_exists(const std::string &member_name) const;

  public:
      Function &create_function(const std::string &member_name);
      Minion &create_minion(const std::string &name, const Profession &profession);

      std::map<std::string, Member_Pointer> &get_members() {
        return members;
      }

      const std::map<std::string, Member_Pointer> &get_members() const {
        return members;
      }
  };
}