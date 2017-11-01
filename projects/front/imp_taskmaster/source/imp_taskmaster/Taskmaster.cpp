#include <imp_artisan/Artisan.h>
#include "Taskmaster.h"
#include <iostream>
#include <fstream>
#include <imp_rendering/headers.h>
#include <imp_rendering/sources.h>
#include <overworld_exploring/dependencies.h>
#include <boost/filesystem/operations.hpp>
#include <imp_rendering/dependency_gathering.h>

using namespace overworld;
using namespace boost;

namespace imp_taskmaster {

  Taskmaster::~Taskmaster() {

  }

  void Taskmaster::clear_output_directory() {
    filesystem::directory_iterator end_iter;
    for (filesystem::directory_iterator iterator(output_path); iterator != end_iter; ++iterator) {
      if (filesystem::is_directory(iterator->status())) {
        filesystem::remove_all(iterator->path());
//        std::cout << iterator->path().filename() << " [directory]\n";
      }
      else {
        filesystem::remove(iterator->path());
      }
    }
  }

  void Taskmaster::gather_dungeons(const overworld::Dungeon &parent) {
    for (auto &entry : parent.get_scope().get_dungeons()) {
      auto &dungeon = *entry;

      if (dungeon.is_external())
        continue;

      if (dungeon.is_class()) {
        dungeons[dungeon.get_name()] = entry.get();
      }
      gather_dungeons(dungeon);
    }
  }

  void Taskmaster::prepare_header_files() {
    for (auto entry : dungeons) {
      auto &dungeon = *entry.second;
      auto file = new File("", dungeon.get_name() + ".h");
      header_file_map.set(*dungeon.get_reference(), std::unique_ptr<File>(file));
      dungeon.set_file(file);
    }
  }

  void Taskmaster::render_and_write_strokes(const imp_artisan::building::Stroke_Owner &stroke,
                                            const std::string &file_path) {
//    imp_artisan::Artisan artisan;
    auto parent_path = boost::filesystem::path(file_path).parent_path().string();
    boost::filesystem::create_directories(parent_path);
    auto text = stroke.render("");
    std::ofstream output_stream(file_path, std::ios_base::binary | std::ios_base::out);
    if (output_stream.is_open()) {
      output_stream << text;
      output_stream.close();
    }
  }

  void Taskmaster::render_dungeon(const overworld::Dungeon &dungeon) {
    imp_rendering::Include_Manager include_manager(dungeon, header_file_map);
    imp_rendering::set_standard_library(standard_library);
    include_manager.gather_headers(standard_library);

    auto namespace_path = overworld::get_namespace_string(dungeon.get_scope().get_parent_scope()->get_owner(), "/");
    if (namespace_path.size() > 0)
      namespace_path += "/";
    auto header_path = output_path + "/" + namespace_path + dungeon.get_name() + ".h";

    auto header_strokes = imp_rendering::headers::render(dungeon, include_manager.get_header_includes(),
                                                         include_manager.get_forward_declarations());
    render_and_write_strokes(header_strokes, header_path);

    if (imp_rendering::sources::needs_source_file(dungeon)) {
      auto source_path = output_path + "/" + namespace_path + dungeon.get_name() + ".cpp";
      auto source_strokes = imp_rendering::sources::render(dungeon, include_manager.get_source_includes());
      render_and_write_strokes(source_strokes, source_path);
    }
  }

  void Taskmaster::render() {
    gather_dungeons(root);
    prepare_header_files();
    clear_output_directory();

    for (auto &dungeon : dungeons) {
      render_dungeon(*dungeon.second);
    }
  }

}