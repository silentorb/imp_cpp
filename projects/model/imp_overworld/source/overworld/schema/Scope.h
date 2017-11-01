#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "overworld/schema/professions/Profession.h"
#include "Member_Collection.h"
#include <underworld/schema/Function.h>
#include <overworld/expressions/Expression.h>
#include <overworld/imp_graph/Graph.h>

namespace overworld {

  class Function;

  class Scope {
  protected:
      Scope *parent_scope;
      Parent owner;
      std::vector<std::unique_ptr<Function>> functions;
      std::vector<Minion_Owner> minions;
      std::vector<std::unique_ptr<Profession>> professions;
      std::vector<std::unique_ptr<Dungeon>> dungeons;
      Member_Collection member_collection;

  public:
      explicit Scope(Scope *parent, Parent owner);
      ~Scope();
      Scope(const Scope &) = delete;

      void add_function(std::unique_ptr<Function> function);
      Member add_minion(std::unique_ptr<Minion> minion);

      void add_profession(std::unique_ptr<Profession> &profession);
      void add_dungeon(std::unique_ptr<Dungeon> dungeon);

      Member *get_member_or_null(const std::string &name) {
        return member_collection.get_member_or_null(name);
      }

      Member &get_member(const std::string &name) {
        return member_collection.get_member(name);
      }

      void add_member(const std::string &name, Member member) {
        member_collection.add_member(name, member);
      }

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return functions;
      }

      std::map<std::string, Member> &get_members() {
        return member_collection.get_members();
      }

      Function *get_function(const std::string &function_name);

      const std::vector<Minion_Owner> &get_minions() const {
        return minions;
      }

      Minion &get_minion(const std::string &name);
//      virtual Dungeon &get_dungeon();

//      Dungeon *get_dungeon_if_not_function() {
//        return get_function() ? nullptr : &get_dungeon();
//      }

      Scope *get_parent_scope() {
        return parent_scope;
      }

      Parent get_owner() {
        return owner;
      }

      const Parent get_owner() const {
        return owner;
      }

      const Scope *get_parent_scope() const {
        return parent_scope;
      }

//      Function *get_function() {
//        return nullptr;
//      }

      const std::vector<std::unique_ptr<Profession>> &get_professions() const {
        return professions;
      }

      std::vector<std::unique_ptr<Profession>> &get_professions() {
        return professions;
      }

      std::vector<std::unique_ptr<Dungeon>> &get_dungeons() {
        return dungeons;
      }

      const std::vector<std::unique_ptr<Dungeon>> &get_dungeons() const {
        return dungeons;
      }
  };

}