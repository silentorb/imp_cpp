#include "Wrapper_Internal.h"
#include <runic/common/Stream.h>
#include <runic/File_Text_Source.h>
#include <imp_summoning/Summoner.h>
#include <imp_mirror/Mirror.h>
#include <imp_taskmaster/Taskmaster.h>
#include <solving/Solver.h>

namespace imp_wrapper {

  Wrapper_Internal::Wrapper_Internal() :
    underworld_root("", nullptr),
    overworld_root(""),
    overworld_profession_library(graph),
    zookeeper(underworld_root) {

    standard_library = new cpp_stl::Standard_Library();
    standard_library->initialize_overworld(overworld_root, overworld_profession_library, graph);
    standard_library->initialize_underworld(zookeeper);
//    overworld_root.add_dungeon(std::unique_ptr<overworld::Dungeon>(standard_library->get_overworld_dungeon()));
  }

  Wrapper_Internal::~Wrapper_Internal() {
  }

  void Wrapper_Internal::load_file(const std::string &path) {
    zookeeper.load_file(path);
  }

  void Wrapper_Internal::mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager) {
    imp_mirror::Mirror mirror(overworld_profession_library, element_map, graph, temporary_interface_manager);
    mirror.reflect_root(underworld_root, overworld_root);
  }

  void log_node(overworld::Node &node) {
    std::cout << node.get_debug_string();
    std::cout << std::endl;
  }

  int get_row(overworld::Node &node) {
    return node.get_profession_reference().get_source_point().get_row();
  }

  void insert_node(std::vector<overworld::Node *> &nodes, overworld::Node *node) {
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      if (get_row(*node) < get_row(**i)) {
        nodes.insert(i, node);
        return;
      }
    }

    nodes.push_back(node);
  }

  void Wrapper_Internal::solve() {
    solving::Solver solver(graph, overworld_profession_library);
    solver.scan_fresh();
//    solver.log_nodes();
    auto solved = solver.solve();

    solver.log_nodes();

    if (!solved) {
      auto &unknowns = solver.get_unsolved_nodes();
      if (unknowns.size() > 0) {
        auto &unknown = *unknowns[0];
        auto &element = unknown.get_profession_reference();
        throw std::runtime_error("Could not determine type of \"" + element.get_name() +
                                 "\" at " + element.get_source_point().to_string() + ".");
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