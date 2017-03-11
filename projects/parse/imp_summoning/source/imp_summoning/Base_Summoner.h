#pragma once

#include <underworld/schema/Profession_Library.h>
#include "Stream.h"
#include "Lookup.h"

namespace imp_summoning {

  class Base_Summoner {
  protected:
      Stream &input;
      const runic_imp::Symbols &lexicon;
      Lookup &lookup;
      underworld::Profession_Library &profession_library;

  public:
      Base_Summoner(Stream &input, Lookup &lookup);

  };
}
