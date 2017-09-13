#include "Function.h"

namespace underworld {

  Parameter &Virtual_Function::add_parameter(const std::string &name, Profession_Owner profession,
                                             const source_mapping::Source_Point &source) {
    auto minion = new Parameter(name, std::move(profession), source);
    parameters.push_back(std::unique_ptr<Parameter>(minion));
    return *minion;
  }

  Parameter &Function_With_Block::add_parameter(const std::string &name, Profession_Owner profession,
                                                const source_mapping::Source_Point &source) {
    auto minion = new Parameter(name, std::move(profession), source);
    block.get_scope().add_member(std::unique_ptr<Parameter>(minion));
    parameters.push_back(minion);
    return *minion;
  }

}