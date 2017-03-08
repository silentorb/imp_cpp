#pragma once

#include <string>
#include <overworld/schema/Dungeon.h>
#include <imp_artisan/Stroke.h>

namespace imp_taskmaster {

  class Taskmaster {
      const overworld::Dungeon &root;
      const std::string output_path;

      void render_and_write_strokes(imp_artisan::Strokes &strokes, const std::string &file_path);
      void render_dungeon(const overworld::Dungeon &dungeon);

  public:
      Taskmaster(const overworld::Dungeon &root, const std::string &output_path) :
        root(root), output_path(output_path) {}

      void render();
  };
}