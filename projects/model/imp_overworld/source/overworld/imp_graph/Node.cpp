#include "Node.h"

namespace overworld {

  std::string Node::get_debug_string() const {
    auto &profession_reference = get_profession_reference();
    auto &profession = profession_reference.get_profession();
    auto &source_point = profession_reference.get_source_point();

    std::string result = "";
    if (source_point.get_source_file())
      result += std::to_string(source_point.get_row()) + ":" + std::to_string(source_point.get_column()) + " ";

    result += profession_reference.get_name()
              + ":" + profession.get_name();

    if (profession.get_type() == Profession_Type::reference)
      result += "&";
    
    return result;
  }
}