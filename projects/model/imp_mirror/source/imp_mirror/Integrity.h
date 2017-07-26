#pragma once

#include <vector>
#include <overworld/schema/professions/professions.h>

namespace imp_mirror {

  class Integrity {
      std::vector<overworld::Profession_Reference *> unknowns;
  public:

      void check_reference(overworld::Profession_Reference &reference) {
        if (reference.get_profession().get_type() == overworld::Profession_Type::unknown) {
          unknowns.push_back(&reference);
        }
      }

      void verify_no_unknowns();
//      void add_unknown(overworld::Profession_Reference &unknown) {
//        unknowns.push_back(&unknown);
//      }
  };
}