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

  using Member_Array  = std::vector<Member *>;

  class Scope {
      const underworld::Scope *source;

  protected:
      Scope *parent;
      std::vector<std::unique_ptr<Function>> functions;
      std::vector<Variable_Owner> minions;
      std::vector<std::unique_ptr<Profession>> professions;
      std::vector<std::unique_ptr<Dungeon>> dungeons;
      std::map<std::string, Member_Array> members;

      void add_member(const std::string &name, Member &member);

  public:
      Scope(const underworld::Scope *source, Scope *parent);
      Scope(Scope *parent);
      virtual ~Scope();

//      Function &create_function(const underworld::Function &input, Profession &profession);
      Function &create_function(const underworld::Function &input);

//      Minion &create_minion(const underworld::Minion &input, Profession &profession, overworld::Graph &graph);
//      Minion &create_minion(const underworld::Minion &input);
//      Minion &create_minion(const std::string &name, Profession &profession);
      void add_minion(Minion *minion);
      void add_minion(std::unique_ptr<Minion> minion);

      void add_profession(std::unique_ptr<Profession> &profession);
//      void add_profession(std::unique_ptr<Profession> &profession);
      void add_dungeon(std::unique_ptr<Dungeon> &dungeon);
      Member *find_member(const std::string &name);
      Member *get_member_or_null(const std::string &name);
      virtual Member &get_member(const std::string &name);

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return functions;
      }

      virtual Scope_Type get_scope_type() const {
        return Scope_Type::scope;
      }

      Function *get_function(const std::string &function_name);

      const std::vector<Variable_Owner> &get_minions() const {
        return minions;
      }

      virtual Minion &get_minion(const std::string &name);
      virtual Dungeon &get_dungeon();

      Scope *get_parent() {
        return parent;
      }

      const Scope *get_parent() const {
        return parent;
      }

      virtual Function &get_function() {
        throw std::runtime_error("Not supported.");
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

      virtual Function &get_function() override {
        return *function;
      }
  };

//  enum class Scope_Parent_Type {
//      block,
//      dungeon,
//  };
//
//  class Scope_Parent {
//  public:
//      virtual Scope_Parent_Type get_scope_parent_type() const = 0;
//  };
}