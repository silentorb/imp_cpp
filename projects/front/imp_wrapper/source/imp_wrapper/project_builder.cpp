#include <boost/filesystem/operations.hpp>
#include <underworld/schema/Dungeon.h>
#include <imp_taskmaster/project_loader.h>
#include "project_builder.h"

namespace imp_wrapper {

  void load_files(const std::string &path, Wrapper &wrapper, underworld::Dungeon &underworld_root) {
    boost::filesystem::directory_iterator it{path};
    while (it != boost::filesystem::directory_iterator{}) {
      auto &entry = (*it++);
      auto k = entry.path().string();
      if (boost::filesystem::is_directory(entry)) {
        load_files(entry.path().string(), wrapper, underworld_root);
      }
      else if (entry.path().string() == "imp") {
        wrapper.load_file(entry.path().string(), underworld_root);
      }
    }

  }

  void build_project(imp_taskmaster::Project &project, Wrapper &wrapper) {
    if (project.get_source_folder() != "") {
      if (project.get_output_folder() != "") {
        boost::filesystem::create_directories(project.get_output_folder());
        Wrapper wrapper;
        imp_mirror::Temporary_Interface_Manager temporary_interface_manager;
        underworld::Dungeon underworld_root("", nullptr);
        load_files(project.get_source_folder(), wrapper, underworld_root);
        wrapper.mirror(temporary_interface_manager, underworld_root);
        wrapper.solve();
        wrapper.render(project.get_output_folder());
      }
    }
  }

  void build_workspace(const std::string &path) {
    imp_taskmaster::Project_Map all_projects;
    Wrapper wrapper;
    auto project = imp_taskmaster::load_project(path, all_projects);
    for (auto &child : project->get_projects()) {
      build_project(*child, wrapper);
    }
    build_project(*project, wrapper);
  }
}