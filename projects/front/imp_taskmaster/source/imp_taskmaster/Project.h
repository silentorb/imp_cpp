#pragma once

#include <memory>
#include <vector>
#include <string>

namespace imp_taskmaster {

  class Project {
      std::string name;
      std::vector<std::unique_ptr<Project>> projects;
      std::vector<Project *> dependencies;
      std::string source_folder;
      std::string output_folder;

  public:
      Project(const std::string &name) : name(name) {}

      void add_project(std::unique_ptr<Project> project) {
        projects.push_back(std::move(project));
      }

      void add_dependency(Project &project) {
        dependencies.push_back(&project);
      }

      const std::string &get_name() const {
        return name;
      }

      const std::vector<std::unique_ptr<Project>> &get_projects() const {
        return projects;
      }

      const std::vector<Project *> &get_dependencies() const {
        return dependencies;
      }

      const std::string &get_source_folder() const {
        return source_folder;
      }

      void set_source_folder(const std::string &source_folder) {
        Project::source_folder = source_folder;
      }

      const std::string &get_output_folder() const {
        return output_folder;
      }

      void set_output_folder(const std::string &output_folder) {
        Project::output_folder = output_folder;
      }
  };
}