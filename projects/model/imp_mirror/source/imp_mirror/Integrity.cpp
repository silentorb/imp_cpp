#include <stdexcept>
#include "Integrity.h"

namespace imp_mirror {

  void Integrity::verify_no_unknowns() {
    if (unknowns.size() > 0) {
      throw new std::runtime_error("Could not determine type.");
    }
  }
}