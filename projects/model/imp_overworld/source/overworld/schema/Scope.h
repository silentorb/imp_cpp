#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "overworld/schema/professions/Profession.h"
#include "Symbol_Source.h"
#include <underworld/schema/Function.h>
#include <overworld/expressions/Expression.h>
#include <overworld/imp_graph/Graph.h>

namespace overworld {

  class Function;

  enum class Scope_Type {
      scope,
      dungeon
  };

  class Scope {
  protected:
      Scope *parent;
      std::vector<std::unique_ptr<Function>> functions;
      std::vector<Minion_Owner> minions;
      std::vector<std::unique_ptr<Profession>> professions;
      std::vector<std::unique_ptr<Dungeon>> dungeons;
      std::map<std::string, Member_Owner> members;
//      std::vector<Scope *> imported_scopes;

      void use_member_array(const std::string &name, Member_Owner member, Member_Owner &existing);
      void add_member(const std::string &name, Member_Owner member);

  public:
      explicit Scope(Scope *parent);
      virtual ~Scope();

      void add_function(std::unique_ptr<Function> function);
      Member &add_minion(Minion &minion);
      Member &add_minion(std::unique_ptr<Minion> minion);

      void add_profession(std::unique_ptr<Profession> &profession);
      void add_dungeon(std::unique_ptr<Dungeon> dungeon);
//      Member *find_member(const std::string &name);
      Member *get_member_or_null(const std::string &name);
      virtual Member &get_member(const std::string &name);

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return functions;
      }

      virtual Scope_Type get_scope_type() const {
        return Scope_Type::scope;
      }

      Function *get_function(const std::string &function_name);

      const std::vector<Minion_Owner> &get_minions() const {
        return minions;
      }

      virtual Minion &get_minion(const std::string &name);
      virtual Dungeon &get_dungeon();

      Dungeon *get_dungeon_if_not_function() {
        return get_function() ? nullptr : &get_dungeon();
      }

      Scope *get_parent() {
        return parent;
      }

//      void import_scope(Scope &scope) {
//        imported_scopes.push_back(&scope);
//      }

      const Scope *get_parent() const {
        return parent;
      }

      virtual Function *get_function() {
        return nullptr;
      }

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

  class Function_Scope : public Scope {
      Function *function = nullptr;

  public:
      Function_Scope(Scope &parent, Function &function);

      Function *get_function() override {
        return function;
      }
  };
}