#pragma once

#include <string>
#include <overworld/schema/Dungeon.h>
#include <imp_artisan/Stroke.h>
#include <unordered_map>
#include "Profession_File_Map.h"

namespace imp_taskmaster {

  class Taskmaster {
      const overworld::Dungeon &root;
      const std::string output_path;
      std::unordered_map<std::string, overworld::Dungeon *> dungeons;
      Profession_File_Map header_file_map;

      void clear_output_directory();
      void gather_dungeons(const overworld::Dungeon &parent);
      void prepare_header_files();
      void render_and_write_strokes(const imp_artisan::building::Stroke_Owner &stroke, const std::string &file_path);
      void render_dungeon(const overworld::Dungeon &dungeon);

  public:
      Taskmaster(const overworld::Dungeon &root, const std::string &output_path) :
        root(root), output_path(output_path) {}

      virtual ~Taskmaster();
      void render();
  };
}