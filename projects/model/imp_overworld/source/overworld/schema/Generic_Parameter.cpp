#include "Generic_Parameter.h"
#include <overworld/schema/professions/Profession_Library.h>

namespace overworld {

  std::string get_generic_parameter_name(size_t index) {
    return std::string(1, 'A' + (char) index);
  }

  std::unique_ptr<Generic_Parameter> &add_generic_parameter_to_vector(
    std::vector<std::unique_ptr<Generic_Parameter>> &generic_parameters,
    Parent parent) {
    if (generic_parameters.size() > 25)
      throw std::runtime_error("Too many generic parameters.");

    auto parameter_name = get_generic_parameter_name(generic_parameters.size());
    auto parameter = new Generic_Parameter(parameter_name, Profession_Library::get_unknown(),
                                           parent, source_mapping::Source_Range());
    generic_parameters.push_back(std::unique_ptr<Generic_Parameter>(parameter));
    return generic_parameters[generic_parameters.size() - 1];
  }

//  void rename_generic_parameters(std::vector<Generic_Parameter_Owner> &generic_parameters) {
//    size_t i = 0;
//    for (auto &parameter: generic_parameters) {
//      parameter->set_name(get_generic_parameter_name(i++));
//    }
//  }

  std::vector<Profession_Reference> to_professions(const Generic_Parameter_Array &generic_parameters,
                                                   size_t additional_space) {
    std::vector<Profession_Reference> result;
    result.reserve(generic_parameters.size() + additional_space);
    for (auto &parameter: generic_parameters) {
      result.push_back(parameter->get_node().get_profession());
    }

    return result;
  }
}