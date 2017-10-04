#pragma once

#include "Parameter.h"
#include "Profession.h"
#include <vector>

namespace underworld {

  class Function_Signature {
      std::vector<Parameter_Owner> parameters;
      Profession_Owner return_type;

  public:
      Function_Signature() {}

      Function_Signature(Profession_Owner return_type) :
        return_type(std::move(return_type)) {}

      const std::vector<Parameter_Owner> &get_parameters() const {
        return parameters;
      }

      const Profession *get_return_type() const {
        return return_type.get();
      }

      void add_parameter(Parameter_Owner parameter) {
        parameters.push_back(std::move(parameter));
      }
  };

};