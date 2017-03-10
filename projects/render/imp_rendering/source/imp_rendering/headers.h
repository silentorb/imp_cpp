#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>
#include "common.h"

namespace imp_rendering {

  namespace headers {
//    imp_artisan::Stroke *render_dungeon_body(const overworld::Dungeon &dungeon);
//    imp_artisan::Stroke *render_function(const overworld::Function &function);

    Stroke render(const overworld::Dungeon &dungeon);
  }
}