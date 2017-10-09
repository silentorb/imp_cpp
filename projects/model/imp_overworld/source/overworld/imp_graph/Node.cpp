#include "Node.h"
#include <overworld/schema/professions/professions.h>
#include <overworld/schema/Dungeon.h>

namespace overworld {

  std::string render_node_status(Node_Status status) {
    switch (status) {

      case Node_Status::unresolved:
        return "?";

      case Node_Status::partial:
        return "!?";

      default:
        return "";
    }
  }

  std::string Node::get_debug_string() const {
    auto &element = get_element();
    auto &profession = element.get_profession();
    auto &source_point = element.get_source_point().get_start();

    std::string result = "";
    if (source_point.get_source_file())
      result += std::to_string(source_point.get_row()) + ":" + std::to_string(source_point.get_column()) + " ";

    result += element.get_name() + render_node_status(get_status())
              + ":" + profession.get_debug_name();

    if (profession.get_type() == Profession_Type::reference)
      result += dynamic_cast<const Reference *>(&profession)->is_pointer() ? "*" : "&";

    return result;
  }

  Node_Status Element_Reference_Node::get_status() const {
    auto &profession = element.get_profession();
    auto &base_profession = profession.get_base();
    if (base_profession.get_type() == Profession_Type::dungeon) {
      auto dungeon_interface = dynamic_cast<Dungeon_Interface *>(&base_profession);
      auto &dungeon = dungeon_interface->get_original();
      if (!dungeon.get_generic_parameters().empty())
        return Node_Status::unresolved;
    }

    return base_profession.get_type() != Profession_Type::unknown
           && base_profession.get_type() != Profession_Type::generic_parameter
           ? Node_Status::resolved
           : Node_Status::unresolved;
  }
}