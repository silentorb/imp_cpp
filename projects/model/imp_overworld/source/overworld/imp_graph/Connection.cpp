#include <overworld/schema/Dungeon_Variant.h>
#include <overworld/schema/Dungeon_Reference.h>
#include <overworld/schema/Parameter.h>
#include <overworld/schema/Function_Signature.h>
#include "Connection.h"
#include "Node.h"

namespace overworld {

  Profession_Reference Connection::get_profession(Node &node) {
    return node.get_profession();
  }

  Profession_Reference Variant_To_Argument::get_profession(Node &node) {
    auto &first = get_first();
    auto &second = get_second();

    if (&node == &get_first()) {
      auto &compound_profession = first.get_profession();
      auto &dungeon = compound_profession->get_dungeon_interface();
      auto variant = static_cast<Dungeon_Variant *>(&dungeon);
      auto &arguments = variant->get_arguments();
      return arguments[parameter_index]->get_node().get_profession();
    }
    else {
      auto &compound_profession = first.get_profession();
      auto &dungeon = compound_profession->get_dungeon_interface();
      auto variant = static_cast<Dungeon_Variant *>(&dungeon);
      std::vector<overworld::Profession_Reference> professions;
      professions.push_back(second.get_profession());
      auto new_variant = new Dungeon_Variant(dungeon.get_original(), professions);
      auto dungeon_reference = new Dungeon_Reference(Dungeon_Interface_Owner(new_variant));
      return Profession_Reference(dungeon_reference);
    }
  }

  Profession_Reference Lambda_To::get_profession(Node &node) {
    auto &first = get_first();
    auto &second = get_second();

    if (&node == &get_first()) {
      auto &compound_profession = first.get_profession();
      auto &dungeon = compound_profession->get_dungeon_interface();
      auto variant = static_cast<Dungeon_Variant *>(&dungeon);
      auto &arguments = variant->get_arguments();
			auto &new_profession = arguments[parameter_index]->get_node().get_profession();
      auto signature = new overworld::Function_Signature();
      auto result = overworld::Profession_Reference(signature);
      auto &second_profession = second.get_profession().get_base(second.get_profession());
      auto old_signature = static_cast<Function_Signature *>(second_profession.get());
      auto &old_elements = old_signature->get_elements();
      auto &first_old_element = old_elements[0];
      signature->add_element(overworld::Parameter_Owner(new overworld::Parameter(
        first_old_element->get_name(),
        new_profession,
        first_old_element->get_element().get_parent(),
        first_old_element->get_element().get_source_point()
      )));
      for (auto i = old_elements.begin() + 1; i != old_elements.end(); i++) {
        auto &element = **i;
        signature->add_element(overworld::Parameter_Owner(new overworld::Parameter(
          element.get_name(),
          element.get_profession(),
          element.get_element().get_parent(),
          element.get_element().get_source_point()
        )));
      }
      return result;
    }
    else {
      auto &compound_profession = first.get_profession();
      throw std::runtime_error("Not implemented.");
    }
  }
}