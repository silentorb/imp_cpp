#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "professions.h"
#include "Profession_Member.h"

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
      Function &create_function(const std::string &member_name, const Profession &profession,
                                const Source_Point &source);
      Minion &create_minion(const std::string &name, const Profession &profession, const Source_Point &source);
      Profession_Member &add_profession(std::unique_ptr<Profession> &profession, const Source_Point &source);

      Dungeon &create_dungeon(const std::string &name, const Source_Point &source);

      std::map<std::string, Member_Pointer> &get_members() {
        return members;
      }

      const std::map<std::string, Member_Pointer> &get_members() const {
        return members;
      }

      Function *get_function(const std::string &name) const;
  };
}