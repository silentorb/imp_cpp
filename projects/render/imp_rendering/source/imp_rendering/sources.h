#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>
#include "common_rendering.h"

namespace imp_rendering {
  namespace sources {

    Stroke render(const overworld::Dungeon &dungeon, const std::vector<overworld::File_Reference> &files);
    bool needs_source_file(const overworld::Dungeon &dungeon);
  }
}