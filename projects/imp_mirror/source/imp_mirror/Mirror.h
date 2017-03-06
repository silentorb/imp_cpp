#pragma once

#include <overworld/schema/Dungeon.h>
#include <underworld/schema/Dungeon.h>

namespace imp_mirror {

  class Mirror {

  public:
      void reflect(const underworld::Dungeon &input, overworld::Dungeon &output);
  };
}