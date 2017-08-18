#include "Variant.h"

namespace overworld {

  const std::string &Variant::format_name(const std::string &original_name) {
    auto name = original_name + "<";
    auto first = true;
    for (auto profession : professions) {
      if (first)
        first = false;
      else
        name += ", ";

      name += profession->get_name();
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
}