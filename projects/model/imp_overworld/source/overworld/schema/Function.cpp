#include <overworld/exploring/Expression_Explorer.h>
#include "Function.h"
#include "Dungeon.h"
#include "overworld/schema/professions/Profession_Library.h"
#include "Interface.h"

namespace overworld {

  bool Function::is_constructor(const Parent &parent) const {
    if (parent.get_type() == Parent_Type::dungeon)
      return parent.get_dungeon().get_name() == get_name();

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

  bool Function::has_generic_arguments() const {
    for (auto &argument : arguments) {
      if (argument->get_profession()->get_type() == Profession_Type::generic_parameter)
        return true;
    }
    return false;
  }

  Function &Function::get_or_create_variant(Dungeon &containing_dungeon) {
    std::vector<Profession_Reference> new_arguments;
    for (auto &argument : arguments) {
      auto &profession = argument->get_profession();
      if (profession.get_type() == Profession_Type::generic_parameter) {
        auto generic_parameter = dynamic_cast<Generic_Parameter *>(profession.get());
        auto generic_argument = containing_dungeon.get_generic_argument(*generic_parameter);
        new_arguments.push_back(generic_argument->get_profession());
      }
      else {
        new_arguments.push_back(profession);
      }
    }

    for (auto &existing_function : variants) {
      if (existing_function->get_signature().matches(new_arguments))
        return *existing_function;
    }

    auto new_function = new Virtual_Function(*this, new_arguments);
    variants.push_back(std::unique_ptr<Function>(new_function));
    return *new_function;
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