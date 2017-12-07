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

//  bool Function::has_generic_arguments() const {
//    for (auto &argument : arguments) {
//      if (argument->get_profession()->get_type() == Profession_Type::generic_parameter)
//        return true;
//    }
//    return false;
//  }

  Function &Function::get_variant(Dungeon &containing_dungeon) {
    auto new_arguments = get_variant_arguments(containing_dungeon);
    for (auto &existing_function : variants) {
      if (existing_function->get_signature().matches(new_arguments))
        return *existing_function;
    }

    throw std::runtime_error("Not supported.");
  }

  std::vector<Profession_Reference> Function::get_variant_arguments(Dungeon &containing_dungeon) {
    std::vector<Profession_Reference> new_arguments;
    for (auto &element : signature.get_elements()) {
      auto &profession = element->get_profession();
      if (profession.get_type() == Profession_Type::generic_parameter) {
        auto generic_parameter = dynamic_cast<Generic_Parameter *>(profession.get());
        auto generic_argument = containing_dungeon.get_generic_argument(*generic_parameter);
        new_arguments.push_back(generic_argument->get_profession());
      }
      else {
        new_arguments.push_back(profession);
      }
    }
    return new_arguments;
  }

  Function &Function::get_or_create_variant(Dungeon &containing_dungeon, Graph &graph) {
    auto new_arguments = get_variant_arguments(containing_dungeon);

    for (auto &existing_function : variants) {
      if (existing_function->get_signature().matches(new_arguments))
        return *existing_function;
    }

    auto new_function = new Virtual_Function(*this);
    variants.push_back(std::unique_ptr<Function>(new_function));
    auto generic_arguments = containing_dungeon.get_arguments().begin();
    auto original_parameters = signature.get_elements().begin();
    int i = 0;
    for (auto &profession : new_arguments) {
      auto &generic_argument = *generic_arguments++;
      auto &original_parameter = *original_parameters++;
      auto new_parameter = new Parameter(original_parameter->get_name(), profession, *new_function,
                                         original_parameter->get_element().get_source_point());
      new_function->get_signature().add_element(Parameter_Owner(new_parameter));
      if (original_parameter->get_profession().get_type() == Profession_Type::generic_parameter) {
//        auto connection = new Variant_To_Argument(member_container->get_node(), second, i);
//        graph.connect(member_container->get_node(), second, std::unique_ptr<Connection>(connection));
        graph.connect(new_parameter->get_node(), generic_argument->get_node());
      }
      ++i;
    }
    return *new_function;
  }

  void Function::finalize(overworld::Profession_Library &profession_library) {
    set_profession(profession_library.get_void(), Empty_Profession_Setter::get_instance());
  }

}