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
    standard_library = new cpp_stl::Standard_Library(overworld_root);
    overworld_root.add_dungeon(std::unique_ptr<overworld::Dungeon>(standard_library));
    standard_library->initialize(overworld_profession_library);
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
    auto &profession = profession_reference.get_profession();
    auto & source_point =profession_reference.get_source_point();

    if (source_point.get_source_file())
      std::cout << source_point.get_row() << ":" << source_point.get_column() << " ";

    std::cout << profession_reference.get_name()
              << ":" << profession.get_name();

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

  void log_nodes(const std::vector<overworld::Node *> &original_nodes) {
    std::vector<overworld::Node *> nodes;
    for (auto first: original_nodes) {
      insert_node(nodes, first);
    }
    for (auto &node : nodes) {
      log_node(*node);
      for (auto &other : node->get_neighbors()) {
        std::cout << " * ";
        log_node(*other);
      }
    }
  }

  void Wrapper::solve() {
    solving::Solver solver(graph.get_graph());
    solver.scan_fresh();
    std::cout << std::endl;
    auto solved = solver.solve();
    log_nodes(graph.get_graph().get_nodes());

    if (!solved) {
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
    imp_taskmaster::Taskmaster taskmaster(overworld_root, output_path, *standard_library);
    taskmaster.render();
  }
}