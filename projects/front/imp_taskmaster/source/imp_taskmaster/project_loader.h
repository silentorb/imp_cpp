#pragma once

#include <memory>
#include "Project.h"
#include <map>

namespace imp_taskmaster {

  using Project_Map = std::map<std::string, Project *>;

  std::unique_ptr<Project> load_project(const std::string &path, Project_Map &all_projects);
}