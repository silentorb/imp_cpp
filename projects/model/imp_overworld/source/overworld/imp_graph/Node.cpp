#include "Node.h"
#include <overworld/schema/professions/professions.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Function_Signature.h>

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
    auto &profession = get_profession();
    auto &source_point = element.get_source_point().get_start();

    std::string result = "";
    if (source_point.get_source_file())
      result += //source_point.get_source_file()->get_short_file_path() + " " +
                std::to_string(source_point.get_row()) + ":" +
                std::to_string(source_point.get_column()) + " ";

    result += element.get_name() + render_node_status(get_status())
              + ":" + profession->get_debug_name();

    if (profession->get_type() == Profession_Type::reference)
      result += dynamic_cast<const Reference *>(profession.get())->is_pointer() ? "*" : "&";

    return result;
  }

  Node_Status get_status_using_profession(Profession &base_profession) {
    if (base_profession.get_type() == Profession_Type::dungeon) {
      auto dungeon_interface = dynamic_cast<Dungeon_Interface *>(&base_profession);
      auto &dungeon = dungeon_interface->get_original();
      if (!dungeon.get_generic_parameters().empty())
        return Node_Status::unresolved;
      else
        return Node_Status::resolved;
    }

    else if (base_profession.get_type() == Profession_Type::function) {
      auto signature = static_cast<Function_Signature *>(&base_profession);
      bool some_resolved = false;
      bool some_unknown = false;
      for (auto &parameter: signature->get_parameters()) {
        if (get_status_using_profession(*parameter->get_profession()) == Node_Status::resolved) {
          if (some_unknown)
            return Node_Status::partial;

          some_resolved = true;
        }
        else {
          if (some_resolved)
            return Node_Status::partial;

          some_unknown = true;
        }
      }
      return some_resolved
             ? Node_Status::resolved
             : Node_Status::unresolved;
    }
    else {
      return base_profession.get_type() != Profession_Type::unknown
             && base_profession.get_type() != Profession_Type::generic_parameter
             ? Node_Status::resolved
             : Node_Status::unresolved;
    }
  }

//  Node_Status Element_Reference_Node::get_status() const {
//    auto &profession = element.get_profession();
//    auto &base_profession = profession->get_base(profession);
//    return get_status_using_profession(*base_profession);
//  }
}