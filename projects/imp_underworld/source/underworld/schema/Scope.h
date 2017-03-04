#pragma once

#include <string>
#include <map>
#include "Portal.h"
#include "professions.h"

namespace underworld {

  class Minion;

  class Scope {
  protected:
      std::map<std::string, Member_Pointer> members;

      void check_has_member(const std::string &member_name) {
        if (members.count(member_name) != 0)
          throw_already_exists(member_name);
      }

      virtual void throw_already_exists(const std::string &member_name) const;

  public:
      Minion &create_minion(const std::string &name);
      Portal &create_portal(const std::string &name, const  Profession & profession);

  };
}