#include <overworld/exploring/Expression_Explorer.h>
#include "Function.h"
#include "Dungeon.h"
#include "overworld/schema/professions/Profession_Library.h"
#include "Interface.h"

namespace overworld {

  bool Function::is_constructor() const {
    auto &parent = get_parent_scope().get_owner();
    if (parent.get_type() == Parent_Type::dungeon)
      return parent.get_dungeon().get_original().get_name() == get_name();

    return false;
  }

  bool Function_With_Block::is_inline() const {
    return generic_parameters.size() > 0 || block.get_expressions().size() < 2;
  }

  bool Function::returns_a_value() const {
    auto &return_type = signature.get_last().get_profession();
    return return_type.get_type() != Profession_Type::unknown
           && return_type.get_type() != Profession_Type::Void;
  }

  bool Function_With_Block::returns_a_value() const {
    if (signature.get_elements().size() > 0) {
      auto &return_type = signature.get_last().get_profession();
      if (return_type.get_type() != Profession_Type::unknown)
        return true;
    }
    
    bool result = false;

    exploring::Expression_Explorer explorer([&result](const Expression &expression) {
      if (expression.get_type() == Expression_Type::return_with_value)
        result = true;
    });
    explorer.explore_block(block);
    return result;
  }

  void Function::finalize(overworld::Profession_Library &profession_library) {
    set_profession(profession_library.get_void(), Empty_Profession_Setter::get_instance());
//    if (!returns_a_value()) {
//      set_profession(profession_library.get_void(), Empty_Profession_Setter::get_instance());
////      node.set_status(Node_Status::resolved);
//    }
//    else {
//      set_profession(profession_library.get_unknown(), Empty_Profession_Setter::get_instance());
//    }
  }

/*
  overworld::Temporary_Interface &Function_With_Block::get_or_create_interface(Parameter &parameter) {
    if (!temporary_interface_manager) {
      temporary_interface_manager = std::make_unique<Temporary_Interface_Manager>();
    }
    else {
      for (auto &entry : temporary_interface_manager->get_entries()) {
        if (&entry.get_parameter() == &parameter)
          return *entry.get_interface();
      }
    }

    auto dungeon = new Temporary_Interface();
    temporary_interface_manager->add(parameter, dungeon);
    return *dungeon;
  }
*/
//  Minion &Function::create_parameter(const std::string &name, Profession &profession) {
//    auto &minion = scope.create_minion(name, profession);
//    add_parameter(minion);
//    return minion;
//  }

}