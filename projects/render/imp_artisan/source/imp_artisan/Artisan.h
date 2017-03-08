#pragma once

#include <string>
#include "Stroke.h"

namespace imp_artisan {

  class Artisan {

  public:
      std::string render(Strokes &strokes);
  };
}