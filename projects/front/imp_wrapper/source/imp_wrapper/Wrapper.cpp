#include <imp_summoning/Stream.h>
#include <runic/File_Text_Source.h>
#include <imp_summoning/Summoner.h>
#include <imp_mirror/Mirror.h>
#include <imp_taskmaster/Taskmaster.h>
#include <solving/Solver.h>
#include <cpp_stl/Standard_Library.h>
#include "Wrapper.h"
#include <iostream>

namespace imp_wrapper {

  Wrapper::Wrapper() :
    underworld_root("", nullptr),
    lookup(),
    overworld_root("") {
    auto standard_library = new cpp_stl::Standard_Library(overworld_root, overworld_profession_library, graph);
    std::unique_ptr<overworld::Dungeon> library_pointer(standard_library);
    overworld_root.add_dungeon(library_pointer);
  }

  void Wrapper::load_file(const std::string &path) {
    runic_imp::Imp_Lexer lexer(new runic::File_Text_Source<>(path));
    auto source_file = new underworld::Source_File(path);
    source_files.push_back(std::unique_ptr<underworld::Source_File>(source_file));
    imp_summoning::Stream stream(lexer, *source_file);
    imp_summoning::Summoner summoner(stream, lookup);
    summoner.summon(underworld_root);
  }

  void Wrapper::mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager) {
    imp_mirror::Mirror mirror(overworld_profession_library, element_map, graph, temporary_interface_manager);
    mirror.reflect_dungeon(underworld_root, overworld_root);
  }

  void log_node(overworld::Node &node) {
    auto &profession_reference = node.get_profession_reference();
    std::cout << profession_reference.get_name()
              << ":" << profession_reference.get_profession().get_name()
              << " - " << profession_reference.get_source_point().to_string() << std::endl;
  }

  void Wrapper::solve() {
    solving::Solver solver(graph.get_graph());
    std::cout << std::endl;
    for (auto &node : graph.get_graph().get_nodes()) {
      log_node(*node);
      for (auto &other : node->get_neighbors()) {
        std::cout << " * ";
        log_node(*other);
      }
    }
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