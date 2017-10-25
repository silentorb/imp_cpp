#pragma once

#include "Parameter.h"
#include "Profession.h"
#include <vector>

namespace underworld {

  class Function_Signature {
      std::vector<Parameter_Owner> parameters;

  public:
      Function_Signature() {}

      const std::vector<Parameter_Owner> &get_elements() const {
        return parameters;
      }

      void add_parameter(Parameter_Owner parameter) {
        parameters.push_back(std::move(parameter));
      }

      Parameter &get_last() const {
        if (parameters.size() == 0)
          throw std::runtime_error("Can't get last parameter from empty function signature.");

        return *parameters[parameters.size() - 1];
      }
  };

};