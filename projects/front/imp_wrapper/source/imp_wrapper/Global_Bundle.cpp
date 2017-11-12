#include "Global_Bundle.h"
#include <runic/common/Stream.h>
#include <runic/File_Text_Source.h>
#include <imp_summoning/Summoner.h>
#include <imp_mirror/Mirror.h>
#include <imp_taskmaster/Taskmaster.h>
#include <solving/Profession_Solver.h>
#include <solving/Solving_Visualizer.h>
#include <solving/lifetime/Ownership_Solver.h>
#include <solving/lifetime/Ownership_Mirror.h>
#include "Project_Bundle.h"


namespace imp_wrapper {

  Global_Bundle::Global_Bundle() :
//    underworld_root("", nullptr),
    overworld_profession_library() {
    initialize_standard_library();
  }

  Global_Bundle::~Global_Bundle() = default;

  void Global_Bundle::initialize_standard_library() {
    standard_library = new cpp_stl::Standard_Library();
    standard_library->initialize_overworld(overworld_profession_library);
    standard_library->initialize_underworld(zookeeper);
    imp_mirror::Temporary_Interface_Manager temporary_interface_manager;
    imp_mirror::Mirror mirror(overworld_profession_library, element_map, nullptr, temporary_interface_manager,
                              header_files);
    imp_mirror::Scope scope(standard_library->get_overworld_dungeon().get_scope());
    mirror.reflect_root(standard_library->get_underworld_dungeon(), scope);
  }

  void Global_Bundle::load_file(const std::string &path, underworld::Dungeon &underworld_root) {
    zookeeper.load_file(path, underworld_root);
  }

  void Global_Bundle::mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager,
                             Project_Bundle &project_bundle) {
    imp_mirror::Mirror mirror(overworld_profession_library, element_map, &project_bundle.get_graph(),
                              temporary_interface_manager,
                              header_files);
    mirror.reflect_root(project_bundle.get_underworld_root(), project_bundle.get_mirror_scope());

    lifetime::Ownership_Mirror ownership_mirror(project_bundle.get_ownership_graph());
    ownership_mirror.reflect(project_bundle.get_overworld_root());
  }

  void Global_Bundle::solve(overworld::Graph &graph, lifetime::Graph &ownership_graph) {
    solving::Profession_Solver solver(graph, overworld_profession_library);
    solver.scan_fresh();

#if DEBUG_SOLVER > 2
    solving::log_nodes(graph);
#endif

    auto solved = solver.solve();

#if DEBUG_SOLVER > 1
    solving::log_nodes(graph);
#endif

//    auto node = solving::find_node(graph, 10, 9);
//    auto &profession = node->get_profession();
//    auto dungeon = dynamic_cast<overworld::Dungeon *>(&profession);
//    auto l = profession.get_debug_name();
//    auto temp = node->get_debug_string();
//    node->get_debug_string();
    if (!solved) {
      auto &unknowns = solver.get_unsolved_nodes();
      if (unknowns.size() > 0) {
        auto &unknown = *unknowns[0];
        auto &element = unknown.get_element();
        unknown._get_status();
        throw std::runtime_error("Could not determine type of \"" + element.get_name() +
                                 "\" at " + element.get_source_point().get_start().to_string() + ".");
      }

      auto &conflicts = solver.get_conflicts();
      if (conflicts.size() > 0) {
        auto &conflict = conflicts.front();
        throw std::runtime_error(
          solving::get_node_debug_string(conflict.get_connection().get_first()) + " is not compatible with "
          + solving::get_node_debug_string(conflict.get_connection().get_second()));
      }
    }

    lifetime::Ownership_Solver ownership_solver(ownership_graph);
    solving::log_node_trees(ownership_graph);
    ownership_solver.solve();
    solving::log_node_trees(ownership_graph);
  }

  void Global_Bundle::render(const std::string &output_path, overworld::Dungeon &overworld_root) {
    imp_taskmaster::Taskmaster taskmaster(overworld_root, output_path, *standard_library);
    taskmaster.render();
  }

  overworld::Dungeon &Global_Bundle::create_realm(const std::string &name) {
    auto realm = new overworld::Dungeon(name);
    realms[name] = overworld::Dungeon_Owner(realm);
    return *realm;
  }

  std::unique_ptr<Project_Bundle> Global_Bundle::create_project_bundle(const std::string &name) {
    auto &realm = create_realm(name);
    auto bundle = std::unique_ptr<Project_Bundle>(new Project_Bundle(realm));
    auto &scope = bundle->get_mirror_scope();
    scope.add_dungeon(standard_library->get_overworld_dungeon());
    return bundle;
  }

  overworld::Dungeon *Global_Bundle::get_realm(const std::string &name) {
    if (realms.count(name) == 0)
      return nullptr;

    return realms[name].get();
  }
}