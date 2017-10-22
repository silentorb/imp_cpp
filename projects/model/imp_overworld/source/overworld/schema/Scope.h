#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "overworld/schema/professions/Profession.h"
#include <underworld/schema/Function.h>
#include <overworld/expressions/Expression.h>
#include <overworld/imp_graph/Graph.h>

namespace overworld {

  class Function;

  enum class Scope_Type {
      scope,
      dungeon
  };

  using Members = std::vector<Member>;

  class Scope {
  protected:
      Scope *parent_scope;
      Parent owner;
      std::vector<std::unique_ptr<Function>> functions;
      std::vector<Minion_Owner> minions;
      std::vector<std::unique_ptr<Profession>> professions;
      std::vector<std::unique_ptr<Dungeon>> dungeons;
      std::map<std::string, Member> members;
      std::map<std::string, Members> overloaded_members;

      void overload(const std::string &name, Member &member);
      void add_overload(const std::string &name, Member &member);

  public:
      explicit Scope(Scope *parent, Parent owner);
      virtual ~Scope();
      Scope(const Scope &) = delete;

      void add_member(const std::string &name, Member member);
      void add_function(std::unique_ptr<Function> function);
      Member add_minion(std::unique_ptr<Minion> minion);

      void add_profession(std::unique_ptr<Profession> &profession);
      void add_dungeon(std::unique_ptr<Dungeon> dungeon);
      Member *get_member_or_null(const std::string &name);
      Member &get_member(const std::string &name);

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return functions;
      }

      std::map<std::string, Member> &get_members() {
        return members;
      }

      Scope_Type get_scope_type() const {
        return Scope_Type::scope;
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

      Parent get_parent() {
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