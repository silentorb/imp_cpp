#include <boost/filesystem/operations.hpp>
#include <underworld/schema/Dungeon.h>
#include <imp_taskmaster/project_loader.h>
#include <imp_mirror/Temporary_Interface_Manager.h>
#include "project_builder.h"
#include "Project_Bundle.h"

namespace imp_wrapper {

  void load_files(const std::string &path, Global_Bundle &wrapper, underworld::Dungeon &underworld_root) {
    boost::filesystem::directory_iterator it{path};
//    auto empty = boost::filesystem::directory_iterator{};
//    while (it != empty) {
    for (auto &entry : it) {
//      auto &entry = (*it++);
      auto file_path = entry.path().string();
      std::replace(file_path.begin(), file_path.end(), '\\', '/');
      auto k = boost::filesystem::path(file_path).extension();
      if (boost::filesystem::is_directory(file_path)) {
        load_files(file_path, wrapper, underworld_root);
      }
      else if (boost::filesystem::path(file_path).extension().string() == ".imp") {
        wrapper.load_file(file_path, underworld_root);
      }
    }
  }

  void build_project(imp_taskmaster::Project &project, Global_Bundle &wrapper) {
    if (project.get_source_folder() != "") {
      imp_mirror::Temporary_Interface_Manager temporary_interface_manager;
      auto project_bundle = wrapper.create_project_bundle(project.get_name());
      auto &scope = project_bundle->get_mirror_scope();
      for (auto &dependency : project.get_dependencies()) {
        auto realm = wrapper.get_realm(dependency->get_name());
        if (!realm)
          throw std::runtime_error("Could not find realm: " + dependency->get_name());

        scope.add_dungeon(*realm);
      }
      load_files(project.get_source_folder(), wrapper, project_bundle->get_underworld_root());
      wrapper.mirror(temporary_interface_manager, *project_bundle);
      wrapper.solve(project_bundle->get_graph(), project_bundle->get_ownership_graph());

      if (project.get_output_folder() != "") {
        boost::filesystem::create_directories(project.get_output_folder());
        wrapper.render(project.get_output_folder(), project_bundle->get_overworld_root());
      }
    }
  }

  void build_workspace(const std::string &path) {
    imp_taskmaster::Project_Map all_projects;
    Global_Bundle wrapper;
    auto project = imp_taskmaster::load_project(path, all_projects);
    for (auto &child : project->get_projects()) {
      build_project(*child, wrapper);
    }
    build_project(*project, wrapper);
  }
}