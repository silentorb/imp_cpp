#include "Wrapper_Internal.h"
#include <runic/common/Stream.h>
#include <runic/File_Text_Source.h>
#include <imp_summoning/Summoner.h>
#include <imp_mirror/Mirror.h>
#include <imp_taskmaster/Taskmaster.h>
#include <solving/Solver.h>
#include <solving/Solving_Visualizer.h>

namespace imp_wrapper {

  Wrapper_Internal::Wrapper_Internal() :
    underworld_root("", nullptr),
    overworld_root(""),
    overworld_profession_library(graph) {

    standard_library = new cpp_stl::Standard_Library();
    standard_library->initialize_overworld(overworld_root, overworld_profession_library, graph);
    standard_library->initialize_underworld(zookeeper);
    imp_mirror::Mirror mirror(overworld_profession_library, element_map, graph, );
    mirror.reflect_root(underworld_root, overworld_root);
  }

  Wrapper_Internal::~Wrapper_Internal() = default;

  void Wrapper_Internal::load_file(const std::string &path, underworld::Dungeon underworld_root) {
    zookeeper.load_file(path, underworld_root);
  }

  void Wrapper_Internal::mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager) {
    imp_mirror::Mirror mirror(overworld_profession_library, element_map, graph, temporary_interface_manager);
    mirror.reflect_root(underworld_root, overworld_root);
  }

  void Wrapper_Internal::solve() {
    solving::Solver solver(graph, overworld_profession_library);
    solver.scan_fresh();

#if DEBUG_SOLVER > 2
    solving::log_nodes(graph);
#endif

    auto solved = solver.solve();

#if DEBUG_SOLVER > 1
    solving::log_nodes(graph);
#endif

    auto node = solving::find_node(graph,  10, 9);
    auto &profession = node->get_element().get_profession();
    auto dungeon = dynamic_cast<overworld::Dungeon*>(&profession);
    auto l = profession.get_debug_name();
    auto temp = node->get_debug_string();
    node->get_debug_string();
    if (!solved) {
      auto &unknowns = solver.get_unsolved_nodes();
      if (unknowns.size() > 0) {
        auto &unknown = *unknowns[0];
        auto &element = unknown.get_element();
        throw std::runtime_error("Could not determine type of \"" + element.get_name() +
                                 "\" at " + element.get_source_point().get_start().to_string() + ".");
      }

      auto &conflicts = solver.get_conflicts();
      if (conflicts.size() > 0) {
        auto &conflict = conflicts.front();
        throw std::runtime_error(conflict.get_connection().get_first().get_debug_string() + " is not compatible with "
                                 + conflict.get_connection().get_second().get_debug_string());
      }
    }
  }

  void Wrapper_Internal::render(const std::string &output_path) {
    imp_taskmaster::Taskmaster taskmaster(overworld_root, output_path, *standard_library);
    taskmaster.render();
  }

}