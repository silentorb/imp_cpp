#pragma once

#include "Dungeon.h"
#include "Variable.h"

namespace overworld {

  class Parameter : public Variable {
      Generic_Argument *generic_argument = nullptr;

  public:
      Parameter(const std::string &name, Profession_Reference &profession, Parent parent,
                const source_mapping::Source_Range &source_range) :
        Variable(Element_Type::parameter, name, profession, parent, source_range) {}

      ~Parameter() {}

      Generic_Argument *get_generic_argument() const {
        return generic_argument;
      }

      void set_generic_argument(Generic_Argument *generic_argument) {
        Parameter::generic_argument = generic_argument;
      }
  };

  using Parameter_Owner = std::unique_ptr<Parameter>;

}