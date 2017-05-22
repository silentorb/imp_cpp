#include "Function.h"

namespace underworld {

  Minion & Function::add_parameter(const std::string &name, const Profession &profession, const Source_Point &source) {
    auto &minion = block.get_scope().create_minion(name, profession, source);
    parameters.push_back(& minion);
    return minion;
  }
}