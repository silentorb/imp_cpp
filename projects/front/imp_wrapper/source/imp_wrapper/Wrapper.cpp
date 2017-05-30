#include <imp_summoning/Stream.h>
#include <runic/File_Text_Source.h>
#include <imp_summoning/Summoner.h>
#include <imp_mirror/Mirror.h>
#include <imp_taskmaster/Taskmaster.h>
#include <solving/Solver.h>
#include <cpp_stl/Standard_Library.h>
#include "Wrapper.h"

namespace imp_wrapper {

  Wrapper::Wrapper() :
    underworld_root("", nullptr),
    lookup(underworld_profession_library),
    overworld_root(underworld_root) {
    auto standard_library = new cpp_stl::Standard_Library(underworld_root);
    std::unique_ptr<underworld::Profession> library_pointer(standard_library);
    underworld_root.add_profession(library_pointer, {standard_library->get_source_file(), 0, 0});
  }

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

  void Wrapper::solve() {
    solving::Solver solver(graph.get_graph());
    solver.scan_fresh();
    if (!solver.solve()) {
      auto &unknowns = solver.get_unsolved_nodes();
      if (unknowns.size() > 0) {
        auto &unknown = *unknowns[0];
        auto &element = unknown.get_profession_reference();
        auto temp = this->source_files[0].get();
        throw std::runtime_error("Could not determine type of \"" + element.get_name() +
                                 "\" at " + element.get_source_point().to_string() + ".");
      }
    }
  }

  void Wrapper::render(const std::string &output_path) {
    imp_taskmaster::Taskmaster taskmaster(overworld_root, output_path);
    taskmaster.render();
  }
}