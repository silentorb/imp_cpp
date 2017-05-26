#include "Function.h"
#include "Dungeon.h"
#include "Profession_Library.h"

namespace overworld {

  bool Function::is_constructor() const {
    if (parent_scope.get_parent().get_scope_parent_type() == Scope_Parent_Type::dungeon) {
      auto &dungeon = *dynamic_cast<Dungeon *>(&parent_scope.get_parent());
      return dungeon.get_name() == get_name();
    }

    return false;
  }

  bool Function::is_inline() {
    return block.get_expressions().size() < 3;
  }

  bool Function::returns_a_value() const {
    for (auto &connection: node.get_connections()) {
      if (&connection->get_first() == &node)
        return true;
    }

    return false;
  }

  void Function::finalize(overworld::Profession_Library &profession_library) {
    if (!returns_a_value()) {
      set_profession(profession_library.get_primitive(Primitive_Type::Void));
      node.set_resolved(true);
    }
  }
}