#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>
#include "common.h"

namespace imp_rendering {

  namespace headers {

    Stroke render(const overworld::Dungeon &dungeon);
  }
}