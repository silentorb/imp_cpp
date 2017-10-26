#include "Node.h"
#include <overworld/schema/professions/professions.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Function_Signature.h>
#include <overworld/schema/Dungeon_Reference.h>
#include <overworld/schema/Dungeon_Variant.h>

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
              + ":" + profession.get_debug_name();

    if (profession.get_type() == Profession_Type::reference)
      result += dynamic_cast<const Reference *>(&profession)->is_pointer() ? "*" : "&";

    return result;
  }

  template<typename Arguments>
  Node_Status get_arguments_status(const Arguments &arguments) {
    bool some_resolved = false;
    bool some_unknown = false;
    for (auto &argument : arguments) {
      const auto &a = *argument.get();
      auto &profession = a.get_profession();
      if (get_status_using_profession(profession) == Node_Status::resolved) {
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

  Node_Status get_status_using_profession(const Profession &profession) {
    switch (profession.get_type()) {
      case Profession_Type::dungeon: {
        auto dungeon_reference = static_cast<const Dungeon_Reference *>(&profession);
        auto &dungeon = dungeon_reference->get_dungeon();
        auto variant = dynamic_cast<const Dungeon_Variant *>(&dungeon);
        if (variant) {
          return get_arguments_status(variant->get_arguments());
        }
        else {
          return Node_Status::resolved;
        }
      }

      case Profession_Type::function: {
        auto signature = static_cast<const Function_Signature *>(&profession);
        return get_arguments_status(signature->get_elements());
      }

      case Profession_Type::unknown:
//      case Profession_Type::Void:
      case Profession_Type::generic_parameter:
        return Node_Status::unresolved;

      default:
        return Node_Status::resolved;
    }
  }

  static Empty_Profession_Setter setter;

  Empty_Profession_Setter &Empty_Profession_Setter::get_instance() {
    return setter;
  }

  void Empty_Profession_Setter::set_profession(overworld::Node &node, overworld::Profession_Reference &profession) {
    node.set_profession(profession, *this);
  }
//  Node_Status Element_Reference_Node::get_status() const {
//    auto &profession = element.get_profession();
//    auto &base_profession = profession->get_base(profession);
//    return get_status_using_profession(*base_profession);
//  }
}