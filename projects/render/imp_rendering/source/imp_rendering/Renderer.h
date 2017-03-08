#pragma once

#include <imp_artisan/Stroke.h>
#include <overworld/schema/Dungeon.h>

namespace imp_rendering {

  class Renderer {

  public:
      void render_header(const overworld::Dungeon &dungeon, imp_artisan::Strokes &strokes);
      void render_source(const overworld::Dungeon &dungeon, imp_artisan::Strokes &strokes);
  };
}