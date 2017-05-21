#include <imp_summoning/Stream.h>
#include <runic/File_Text_Source.h>
#include <imp_summoning/Summoner.h>
#include <imp_mirror/Mirror.h>
#include <imp_taskmaster/Taskmaster.h>
#include "Wrapper.h"

namespace imp_wrapper {

  void Wrapper::load_file(const std::string &path) {
    runic_imp::Imp_Lexer lexer(new runic::File_Text_Source<>(path));
    auto source_file = new underworld::Source_File(path);
    source_files.push_back(std::unique_ptr<underworld::Source_File>(source_file));
    imp_summoning::Stream stream(lexer, *source_file);
    imp_summoning::Summoner summoner(stream, lookup);
    summoner.summon(underworld_root);
  }

  void Wrapper::mirror() {
    imp_mirror::Mirror mirror(overworld_profession_library, element_map, graph);
    mirror.reflect_dungeon(underworld_root, overworld_root);
  }

  void Wrapper::render(const std::string &output_path) {
    imp_taskmaster::Taskmaster taskmaster(overworld_root, output_path);
    taskmaster.render();
  }
}