#pragma once

#include "Dungeon.h"
#include "Variable.h"

namespace overworld {

  class Parameter : public Variable {

  public:
      Parameter(const std::string &name, Profession_Reference &profession, Parent parent,
                const source_mapping::Source_Range &source_range) :
        Variable(Element_Type::parameter, name, profession, parent, source_range) {}

      ~Parameter() {}

  };

  using Parameter_Owner = std::unique_ptr<Parameter>;

}