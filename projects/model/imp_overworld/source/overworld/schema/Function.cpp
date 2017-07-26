#include <overworld/exploring/Expression_Explorer.h>
#include "Function.h"
#include "Dungeon.h"
#include "overworld/schema/professions/Profession_Library.h"

namespace overworld {

  bool Function::is_constructor() const {
    auto dungeon = dynamic_cast<const Dungeon *>(scope.get_parent());
    if (dungeon) {
      return dungeon->get_name() == get_name();
    }

    return false;
  }

  bool Function::is_inline() {
    return block.get_expressions().size() < 2;
  }

  bool Function::returns_a_value() const {
//    for (auto &connection: node.get_connections()) {
//      if (&connection->get_first() == &node)
//        return true;
//    }

    bool result = false;

    exploring::Expression_Explorer explorer([&result](const Expression &expression) {
      if (expression.get_type() == Expression::Type::return_with_value)
        result = true;
    });
    explorer.explore_block(block);
    return result;
  }

  void Function::finalize(overworld::Profession_Library &profession_library) {
    if (!returns_a_value()) {
      set_profession(profession_library.get_void());
      node.set_resolved(true);
    }
    else {
      set_profession(profession_library.get_unknown());
    }
  }

  Minion &Function::create_parameter(const std::string &name, const Profession &profession) {
    auto &minion = scope.create_minion(name, profession);
    add_parameter(minion);
    return minion;
  }
}