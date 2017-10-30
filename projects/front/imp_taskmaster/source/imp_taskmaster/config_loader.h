#pragma once
#include <memory>
#include "Configuration.h"

namespace imp_taskmaster {

  std::unique_ptr<Configuration> load_config(const std::string &path);
}