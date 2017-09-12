#include <overworld/exploring/Expression_Explorer.h>
#include "Function.h"
#include "Dungeon.h"
#include "overworld/schema/professions/Profession_Library.h"

namespace overworld {

  bool Virtual_Function::is_constructor() const {
    auto dungeon = dynamic_cast<const Dungeon *>(scope.get_parent());
    if (dungeon) {
      return dungeon->get_name() == get_name();
    }

    return false;
  }

  bool Function_With_Block::is_inline() const {
    return generic_parameters.size() > 0 || block.get_expressions().size() < 2;
  }

  bool Virtual_Function::returns_a_value() const {
    auto return_type = signature.get_return_type();
    return return_type && return_type->get_type() != Profession_Type::unknown;
  }

  bool Function_With_Block::returns_a_value() const {
    auto return_type = signature.get_return_type();
    if (return_type && return_type->get_type() != Profession_Type::unknown)
      return true;

    bool result = false;

    exploring::Expression_Explorer explorer([&result](const Expression &expression) {
      if (expression.get_type() == Expression::Type::return_with_value)
        result = true;
    });
    explorer.explore_block(block);
    return result;
  }

  void Virtual_Function::finalize(overworld::Profession_Library &profession_library) {
    if (!returns_a_value()) {
      set_profession(profession_library.get_void());
      node.set_resolved(true);
    }
    else {
      set_profession(profession_library.get_unknown());
    }
  }

//  Minion &Function::create_parameter(const std::string &name, Profession &profession) {
//    auto &minion = scope.create_minion(name, profession);
//    add_parameter(minion);
//    return minion;
//  }

}