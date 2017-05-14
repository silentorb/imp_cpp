#include <stdexcept>
#include "Integrity.h"

namespace imp_mirror {

  void Integrity::verify_no_unknowns() {
    if (unknowns.size() > 0) {
      auto &unknown = *unknowns[0];
      throw std::runtime_error("Could not determine type of \"" + unknown.get_name() +
                               "\" at " + unknown.get_source_point().to_string() + ".");
    }
  }
}