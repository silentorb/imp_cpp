#include <imp_rendering/Renderer.h>
#include <imp_artisan/Artisan.h>
#include "Taskmaster.h"
#include <iostream>
#include <fstream>

namespace imp_taskmaster {
  void Taskmaster::render_and_write_strokes(imp_artisan::Strokes &strokes, const std::string &file_path) {
    imp_artisan::Artisan artisan;
    auto text = artisan.render(strokes);
    std::ofstream output_stream(file_path);
    if (output_stream.is_open()) {
      output_stream << text;
      output_stream.close();
    }
  }

  void Taskmaster::render_dungeon(const overworld::Dungeon &dungeon) {
    imp_rendering::Renderer renderer;
    imp_artisan::Strokes header_strokes;
    imp_artisan::Strokes source_strokes;
    renderer.render_header(dungeon, header_strokes);
    renderer.render_source(dungeon, source_strokes);
    render_and_write_strokes(header_strokes, output_path + "/" + dungeon.get_name() + ".h");
    render_and_write_strokes(source_strokes, output_path + "/" + dungeon.get_name() + ".cpp");
  }

  void Taskmaster::render() {
    for (auto &dungeon: root.get_dungeons()) {
      render_dungeon(*dungeon);
    }
  }
}