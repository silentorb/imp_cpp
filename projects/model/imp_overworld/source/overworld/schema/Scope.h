#pragma once

#include <string>
#include <map>
#include "Minion.h"
#include "professions.h"
#include <underworld/schema/Function.h>
#include <overworld/expressions/Expression.h>

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

      Function &create_function(const underworld::Function &input);
      Minion &create_minion(const underworld::Minion &input, const Profession &profession);

      const std::vector<std::unique_ptr<Function>> &get_functions() const {
        return functions;
      }

      const std::vector<Variable_Owner> &get_variables() const {
        return variables;
      }

      Minion &get_variable(const std::string &name);

      Scope_Parent &get_parent() {
        return parent;
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