#include <imp_summoning/Stream.h>
#include <runic/File_Text_Source.h>
#include <imp_summoning/Summoner.h>
#include <imp_mirror/Mirror.h>
#include <imp_taskmaster/Taskmaster.h>
#include "Wrapper.h"

namespace imp_wrapper {

  void Wrapper::load_file(const std::string &path) {
    runic_imp::Imp_Lexer lexer(new runic::File_Text_Source<>(path));
    summoning::Stream stream(lexer);
    summoning::Summoner summoner(stream, underworld_profession_library);
    summoner.summon(underworld_root);
  }

  void Wrapper::mirror() {
    imp_mirror::Mirror mirror(overworld_profession_library);
    mirror.reflect_dungeon(underworld_root, overworld_root);
  }

  void Wrapper::render(const std::string &output_path) {
    imp_taskmaster::Taskmaster taskmaster(overworld_root, output_path);
    taskmaster.render();
  }
}