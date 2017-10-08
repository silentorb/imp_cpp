#pragma once

#include "Parameter.h"
#include "professions/Profession.h"

namespace overworld {

  class Function_Signature {
      std::vector<Parameter_Owner> parameters;
      Node &node;
      Profession *return_type = nullptr;

  public:
      Function_Signature(Node &node, Profession *return_type = nullptr) :
        node(node), return_type(return_type) {}

      const std::vector<Parameter_Owner> &get_parameters() const {
        return parameters;
      }

      Profession *get_return_type() const {
        return return_type;
      }

      void add_parameter(Parameter_Owner parameter) {
        parameters.push_back(std::move(parameter));
      }

      void set_return_type(Profession &profession) {
        return_type = &profession;
      }

      Node &get_node() {
        return node;
      }
  };

}