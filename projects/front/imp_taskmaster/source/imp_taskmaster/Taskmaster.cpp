#include <imp_artisan/Artisan.h>
#include "Taskmaster.h"
#include <iostream>
#include <fstream>
#include <imp_rendering/headers.h>
#include <imp_rendering/sources.h>
#include <overworld_exploring/dependencies.h>

namespace imp_taskmaster {
  void Taskmaster::render_and_write_strokes(const imp_artisan::building::Stroke_Owner &stroke,
                                            const std::string &file_path) {
    imp_artisan::Artisan artisan;
    auto text = artisan.render(stroke);
    std::ofstream output_stream(file_path, std::ios_base::binary | std::ios_base::out);
    if (output_stream.is_open()) {
      output_stream << text;
      output_stream.close();
    }
  }

  void Taskmaster::render_dungeon(const overworld::Dungeon &dungeon) {
    std::vector<overworld::File *> header_files;
    overworld::exploring::gather_header_dependencies(header_files, dungeon);

    auto header_strokes = imp_rendering::headers::render(dungeon, header_files);
    auto source_strokes = imp_rendering::sources::render(dungeon);

    render_and_write_strokes(header_strokes, output_path + "/" + dungeon.get_name() + ".h");
    render_and_write_strokes(source_strokes, output_path + "/" + dungeon.get_name() + ".cpp");
  }

  void Taskmaster::render() {
    for (auto &dungeon : root.get_dungeons()) {
      render_dungeon(*dungeon);
    }
  }
}