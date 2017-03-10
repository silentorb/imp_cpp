#include <imp_artisan/Artisan.h>
#include "Taskmaster.h"
#include <iostream>
#include <fstream>
#include <imp_rendering/headers.h>
#include <imp_rendering/sources.h>

namespace imp_taskmaster {
  void Taskmaster::render_and_write_strokes(const imp_artisan::building::Stroke_Owner &stroke,
                                            const std::string &file_path) {
    imp_artisan::Artisan artisan;
    auto text = artisan.render(stroke);
    std::ofstream output_stream(file_path);
    if (output_stream.is_open()) {
      output_stream << text;
      output_stream.close();
    }
  }

  void Taskmaster::render_dungeon(const overworld::Dungeon &dungeon) {
//    imp_artisan::Stroke_Stream header_strokes;
//    imp_artisan::Stroke_Stream source_strokes;
    auto header_strokes = imp_rendering::headers::render(dungeon);
    auto source_strokes = imp_rendering::sources::render(dungeon);
    render_and_write_strokes(header_strokes, output_path + "/" + dungeon.get_name() + ".h");
    render_and_write_strokes(source_strokes, output_path + "/" + dungeon.get_name() + ".cpp");
  }

  void Taskmaster::render() {
    for (auto &dungeon: root.get_dungeons()) {
      render_dungeon(*dungeon);
    }
  }
}