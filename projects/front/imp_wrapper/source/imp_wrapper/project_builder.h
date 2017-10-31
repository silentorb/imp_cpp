#pragma once

#include <imp_taskmaster/Project.h>
#include "Global_Bundle.h"

namespace imp_wrapper {

  void build_project(imp_taskmaster::Project &project, Global_Bundle &wrapper);
  void build_workspace(const std::string &path);
}
