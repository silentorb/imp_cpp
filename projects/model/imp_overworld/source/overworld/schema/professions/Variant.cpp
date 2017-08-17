#include "Variant.h"

namespace overworld {

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
};