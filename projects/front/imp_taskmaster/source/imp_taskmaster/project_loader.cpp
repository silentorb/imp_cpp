#include "project_loader.h"
#include <yaml-cpp/yaml.h>
#include <boost/filesystem/operations.hpp>
#include <algorithm>

std::string get_absolute_path(std::string path, std::string base) {
  boost::filesystem::path base_path(base);
  while(path.substr(0, 2) == "..") {
    path = path.substr(3);
    base_path = base_path.parent_path();
  }
  auto result = boost::filesystem::absolute(boost::filesystem::path(path), base_path).string();
  std::replace(result.begin(), result.end(), '\\', '/');
  return result;
}

namespace imp_taskmaster {

  std::unique_ptr<Project> load_project(const std::string &path, Project_Map &all_projects) {
    YAML::Node node = YAML::LoadFile(path + "/arch.yaml");
    if (!node["name"].IsScalar())
      throw std::runtime_error("Missing string property name.");

    auto name = node["name"].as<std::string>();
    if (name == "")
      throw std::runtime_error("Property name cannot be empty.");

    if (all_projects.count(name) > 0)
      throw std::runtime_error("A project named " + name + " already exists.");

    auto project = std::make_unique<Project>(name);
    all_projects[name] = project.get();

    auto projects = node["projects"];
    if (projects.IsSequence()) {
      for (auto it = projects.begin(); it != projects.end(); ++it) {
        auto child_path = get_absolute_path(it->as<std::string>(), path);
        auto child_project = load_project(child_path, all_projects);
        project->add_project(std::move(child_project));
      }
    }

    auto dependencies = node["dependencies"];
    if (dependencies.IsSequence()) {
      for (auto it = dependencies.begin(); it != dependencies.end(); ++it) {
        auto name = it->as<std::string>();
        if (all_projects.count(name) == 0)
          throw std::runtime_error("There is no project named " + name + ".");

        project->add_dependency(*all_projects[name]);
      }
    }

    auto build_config = node["build"];
    if (build_config.IsMap()) {
      auto imp_config = build_config["imp"];
      if (imp_config.IsMap()) {

        auto output_folder = imp_config["output"];
        if (output_folder.IsScalar()) {
          project->set_output_folder(get_absolute_path(output_folder.as<std::string>(), path));
        }

        auto source_folder = imp_config["source"];
        if (source_folder.IsScalar()) {
          project->set_source_folder(get_absolute_path(source_folder.as<std::string>(), path));
        }
      }
    }

    return std::move(project);
  }

}