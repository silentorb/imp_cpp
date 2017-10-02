#include "Node.h"
#include <overworld/schema/professions/professions.h>

namespace overworld {

  std::string Node::get_debug_string() const {
    auto &profession_reference = get_element();
    auto &profession = profession_reference.get_profession();
    auto &source_point = profession_reference.get_source_point().get_start();

    std::string result = "";
    if (source_point.get_source_file())
      result += std::to_string(source_point.get_row()) + ":" + std::to_string(source_point.get_column()) + " ";

    result += profession_reference.get_name()
              + ":" + profession.get_name();

    if (profession.get_type() == Profession_Type::reference)
      result += dynamic_cast<const Reference *>(&profession)->is_pointer() ? "*" : "&";

    return result;
  }
}