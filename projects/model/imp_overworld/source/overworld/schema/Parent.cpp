#include <string>
#include "Parent.h"
#include "Dungeon.h"
#include "Function.h"

namespace overworld {

  const std::string Parent::get_name() const {
    if (type == Parent_Type::dungeon)
      return dungeon->get_original().get_name();

    if (type == Parent_Type::function)
      return function->get_original().get_name();

    throw std::runtime_error("Cannot get parent name.");
  }

  Parent *Parent::get_parent() const {
    if (type == Parent_Type::dungeon) {
      auto scope = dungeon->get_original().get_scope().get_parent_scope();
      return scope
             ? &scope->get_parent_scope()->get_owner()
             : nullptr;
    }
    else {
      throw std::runtime_error("Not implemented.");
    }
  }

  const std::string get_namespace_string(const Parent &parent, const std::string &delimiter) {
    if (parent.get_type() == Parent_Type::dungeon) {
      if (parent.get_name() != "") {
        auto parent2 = parent.get_parent();
        return parent2
               ? get_namespace_string(*parent2, delimiter) + delimiter + parent2->get_name()
               : parent.get_name();
      }
    }

    return "";
  }
}
