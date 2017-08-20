#include "Generic_Parameter.h"

namespace overworld {

  Generic_Parameter &add_generic_parameter_to_vector(std::vector<Generic_Parameter_Owner> &generic_parameters,
                                                     Dungeon_Interface &dungeon,
                                                     Function_Interface *function) {
    if (generic_parameters.size() > 25)
      throw std::runtime_error("Too many generic parameters.");

    auto parameter_name = std::string(1, 'A' + (char) generic_parameters.size());
    auto parameter = new Contained_Generic_Parameter(parameter_name, dungeon, function);
    generic_parameters.push_back(std::unique_ptr<Generic_Parameter>(parameter));
    return *parameter;
  }
}