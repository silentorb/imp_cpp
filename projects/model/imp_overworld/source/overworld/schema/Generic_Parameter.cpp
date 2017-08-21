#include "Generic_Parameter.h"

namespace overworld {

  std::string get_generic_parameter_name(size_t index) {
    return std::string(1, 'A' + (char) index);
  }

  Generic_Parameter &add_generic_parameter_to_vector(std::vector<Generic_Parameter_Owner> &generic_parameters,
                                                     Dungeon_Interface &dungeon,
                                                     Function_Interface *function) {
    if (generic_parameters.size() > 25)
      throw std::runtime_error("Too many generic parameters.");

    auto parameter_name = get_generic_parameter_name(generic_parameters.size());
    auto parameter = new Generic_Parameter(parameter_name, dungeon, function);
    generic_parameters.push_back(std::unique_ptr<Generic_Parameter>(parameter));
    return *parameter;
  }

  Generic_Parameter &rename_generic_parameters(std::vector<Generic_Parameter_Owner> &generic_parameters) {
    size_t i = 0;
    for (auto &parameter: generic_parameters) {
      parameter->set_name(get_generic_parameter_name(i++));
    }
  }
}