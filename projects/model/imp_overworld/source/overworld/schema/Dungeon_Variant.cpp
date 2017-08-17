#include "Dungeon_Variant.h"

namespace overworld  {

  void Dungeon_Variant::format_name() {
    name = original.get_name() + "<";
    auto first = true;
    for (auto profession : professions) {
      if (first)
        first = false;
      else
        name += ", ";

      name += profession->get_name();
    }
    name += ">";
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