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

//  class Scope_Parent;

  class Scope {
      const underworld::Scope &source;

  protected:
      Scope *parent;
      std::vector<std::unique_ptr<Function>> functions;
      std::vector<Variable_Owner> minions;
      std::vector<std::unique_ptr<const Profession>> professions;
      std::vector<std::unique_ptr<Dungeon>> dungeons;

  public:
      Scope(const underworld::Scope &source, Scope *parent);
      ~Scope();

      Function &create_function(const underworld::Function &input, const Profession &profession,
                                overworld::Graph &graph);
      Minion &create_minion(const underworld::Minion &input, const Profession &profession, overworld::Graph &graph);
      void add_profession(std::unique_ptr<const Profession> &profession);
      void add_dungeon(std::unique_ptr<Dungeon> &dungeon);

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return functions;
      }

      const std::vector<Variable_Owner> &get_minions() const {
        return minions;
      }

      Minion &get_minion(const std::string &name);

      Scope *get_parent() {
        return parent;
      }

      const Scope *get_parent() const {
        return parent;
      }

      virtual Function &get_function() {
        throw std::runtime_error("Not supported.");
      }

      const std::vector<std::unique_ptr<const Profession>> &get_professions() const {
        return professions;
      }

      std::vector<std::unique_ptr<const Profession>> &get_professions() {
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
      Function_Scope(const underworld::Scope &source, Scope &parent, Function &function);

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