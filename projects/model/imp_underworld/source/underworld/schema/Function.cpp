#include "Function.h"

namespace underworld {

  Parameter &Function::add_parameter(const std::string &name, Profession_Owner &profession,
                                     const source_mapping::Source_Point &source) {
    auto minion = new Parameter(name, profession, source);
    block.get_scope().add_minion(std::unique_ptr<Parameter>(minion));
    parameters.push_back(minion);
    return *minion;
  }

}