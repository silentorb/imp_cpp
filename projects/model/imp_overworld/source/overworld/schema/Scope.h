#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "professions.h"
#include <underworld/schema/Function.h>
#include <overworld/expressions/Expression.h>
#include <overworld/imp_graph/Graph.h>

namespace overworld {

  class Function;

  class Scope_Parent;

  class Scope {
      const underworld::Scope &source;
      Scope_Parent &parent;

  protected:
      std::vector<std::unique_ptr<Function>> functions;
      std::vector<Variable_Owner> variables;

  public:
      Scope(const underworld::Scope &source, Scope_Parent &parent);
      ~Scope();

      Function &create_function(const underworld::Function &input, const Profession &profession,
                                overworld::Graph &graph);
      Minion &create_minion(const underworld::Minion &input, const Profession &profession, overworld::Graph &graph);

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return functions;
      }

      const std::vector<Variable_Owner> &get_variables() const {
        return variables;
      }

      Minion &get_minion(const std::string &name);

      Scope_Parent &get_parent() {
        return parent;
      }

      virtual Function &get_function() {
        throw std::runtime_error("Not supported.");
      }
  };

  class Function_Scope : public Scope {
      Function *function = nullptr;

  public:
      Function_Scope(const underworld::Scope &source, Scope_Parent &parent, Function &function);

      virtual Function &get_function() override {
        return *function;
      }
  };


  enum class Scope_Parent_Type {
      block,
      dungeon,
  };

  class Scope_Parent {
  public:
      virtual Scope_Parent_Type get_scope_parent_type() const = 0;
  };
}