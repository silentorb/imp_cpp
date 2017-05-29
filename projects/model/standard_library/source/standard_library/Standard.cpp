#include "Standard.h"

namespace standard_library {

  Standard::Standard() :
    Dungeon("stl"), file("standard_library") {

    create_dungeon("array", {file, 0, 0});
  }
}
