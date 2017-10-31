#pragma once

#include <imp_taskmaster/Project.h>
#include "Wrapper.h"

namespace imp_wrapper {

  void build_project(imp_taskmaster::Project &project, Wrapper &wrapper);
  void build_workspace(const std::string &path);
}
