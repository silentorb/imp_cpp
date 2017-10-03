#include "Variant.h"

namespace overworld {

  const std::string Variant::format_name(const std::string &original_name) {
    auto name = original_name + "<";
    auto first = true;
    for (auto &argument : arguments) {
      if (first)
        first = false;
      else
        name += ", ";

      name += argument->get_element().get_profession().get_name();
    }
    name += ">";

    return name;
  }

  bool professions_match(const std::vector<Profession *> &first, const std::vector<Profession *> &second) {
    if (first.size() != second.size())
      return false;

    auto other = first.begin();
    for (auto profession : second) {
      if (*other != profession)
        return false;

      ++other;
    }

    return true;
  }

  bool professions_match(const Generic_Parameter_Array &first, const std::vector<Profession *> &second) {
    if (first.size() != second.size())
      return false;

    auto other = first.begin();
    for (auto profession : second) {
      if (&(*other)->get_element().get_profession() != profession)
        return false;

      ++other;
    }

    return true;
  }
}