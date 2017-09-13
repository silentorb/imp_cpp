#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "primitives.h"
#include "Profession_Member.h"

namespace underworld {

  class Function_With_Block;

  class Virtual_Function;

  class Dungeon;

  class Scope {
      Scope *parent = nullptr;

  protected:
      std::map<std::string, Member_Owner> members;

      void check_has_member(const std::string &member_name) {
        if (members.count(member_name) != 0)
          throw_already_exists(member_name);
      }

      virtual void throw_already_exists(const std::string &member_name) const;

  public:
      Scope(Scope *parent) : parent(parent) {}

      void add_member(std::unique_ptr<Member> member);
//      Function &create_function(const std::string &member_name, Profession_Owner &profession,
//                                const source_mapping::Source_Point &source);
      Minion &create_minion(const std::string &name, const source_mapping::Source_Point &source);
      Profession_Member &add_profession(std::unique_ptr<Profession> profession,
                                        const source_mapping::Source_Point &source);
//      void add_minion(std::unique_ptr<Minion> minion);

//      Dungeon &create_dungeon(const std::string &name, const source_mapping::Source_Point &source);

      std::map<std::string, Member_Owner> &get_members() {
        return members;
      }

      const std::map<std::string, Member_Owner> &get_members() const {
        return members;
      }

      Virtual_Function *get_function(const std::string &name) const;
      Profession_Member *get_profession(const std::string &name) const;

      Scope *get_parent() {
        return parent;
      }

      virtual Dungeon &get_dungeon();

      Member *get_member(const std::string &member_name) {
        return members.count(member_name) != 0
               ? members[member_name].get()
               : nullptr;
      }

      const Member *get_member(const std::string &member_name) const {
        return members.count(member_name) != 0
               ? nullptr
               : members.at(member_name).get();
      }
  };
}