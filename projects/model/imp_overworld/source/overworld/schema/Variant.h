#pragma once

#include <string>
#include <overworld/schema/professions/Profession.h>
#include <overworld/imp_graph/Node.h>
#include "Generic_Parameter.h"
#include "Generic_Argument.h"

namespace overworld {

  class Variant {
  protected:
      Generic_Parameter_Array &generic_parameters;
      Generic_Argument_Array arguments;
      std::string name;

      const std::string format_name(const std::string &original_name);

  public:
      Variant(const std::string &original_name, Generic_Parameter_Array &generic_parameters,
              std::vector<Profession_Reference> &professions) :
        generic_parameters(generic_parameters) {
        if (generic_parameters.size() != professions.size())
          throw std::runtime_error("Generic parameter / profession count mismatch.");

        for (auto i = 0; i < generic_parameters.size(); ++i) {
          arguments.push_back(Generic_Argument_Owner(new Generic_Argument(*generic_parameters[i], professions[i])));
        }

        name = format_name(original_name);
      }

      virtual ~Variant() {}

      const Generic_Parameter_Array &get_generic_parameters() const {
        return generic_parameters;
      }

      Generic_Argument_Array &get_arguments() {
        return arguments;
      }

      const Generic_Argument_Array &get_arguments() const {
        return arguments;
      }

      const std::string get_name() const {
        return name;
      }
  };

  bool professions_match(const std::vector<Profession_Reference> &first, const std::vector<Profession_Reference> &second);
  bool professions_match(const Generic_Parameter_Array &first, const std::vector<Profession_Reference> &second);
}