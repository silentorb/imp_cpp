#include "Profession.h"

namespace overworld {

  bool can_cast(Profession &target, Profession &source) {
    if (&target.get_base() == &source.get_base())
      return true;

    auto contracts_or_null = source.get_contracts();
    if (contracts_or_null) {
      auto contracts = *contracts_or_null;
      for (auto contract:  contracts) {
        if (can_cast(target, *contract))
          return true;
      }
    }

    return false;
  }
}