#include "Function.h"
#include "Dungeon.h"

namespace overworld {

  bool Function::is_constructor() const {
    if (parent_scope.get_parent().get_scope_parent_type() == Scope_Parent_Type::dungeon) {
      auto &dungeon = *dynamic_cast<Dungeon *>(&parent_scope.get_parent());
      return dungeon.get_name() == get_name();
    }

    return false;
  }
}