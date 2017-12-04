#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>
#include "common_rendering.h"

namespace imp_rendering {

  namespace headers {

    Stroke render(const overworld::Dungeon &dungeon, const std::vector<overworld::File_Reference> &files,
                  const std::vector<const overworld::Profession *> &forward_declarations);
  }
}