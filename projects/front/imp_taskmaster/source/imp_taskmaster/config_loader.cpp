#include "config_loader.h"
#include <yaml-cpp/yaml.h>

namespace imp_taskmaster {

  std::unique_ptr<Configuration> load_config(const std::string &path) {
    YAML::Node config = YAML::LoadFile(path);

    return std::unique_ptr<Configuration>();
  }
}