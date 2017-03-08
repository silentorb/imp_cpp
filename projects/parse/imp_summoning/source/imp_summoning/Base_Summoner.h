#pragma once

#include <underworld/schema/Profession_Library.h>
#include "Stream.h"

namespace summoning {

  class Base_Summoner {
  protected:
      Stream &input;
      const runic_imp::Symbols &lexicon;
      underworld::Profession_Library &profession_library;

  public:
      Base_Summoner(Stream &input, underworld::Profession_Library &profession_library);

  };
}
