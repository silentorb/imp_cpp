#include "Function_Signature.h"

namespace overworld {

  const std::string Function_Signature::get_debug_name() const {
    std::string result = "(";
    bool first = true;
    for (auto &element : elements) {
      if (first)
        first = false;
      else
        result += " -> ";

      result += element->get_node().get_profession().get_name();
    }

    return result + ")";
  }

  const std::string Function_Signature::get_debug_name_with_names() const {
    std::string result = "(";
    bool first = true;
    for (auto &element : elements) {
      if (first)
        first = false;
      else
        result += " -> ";

      result += element->get_name() != ""
                ? element->get_name()
                : element->get_node().get_profession().get_name();
    }

    return result + ")";
  }
}