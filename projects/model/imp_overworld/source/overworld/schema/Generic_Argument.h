#pragma once

#include "Generic_Parameter.h"

namespace overworld {

  class Generic_Argument {
      Common_Element element;
      Generic_Parameter &parameter;
      Element_Reference_Node node;

  public:
      Generic_Argument(Generic_Parameter &parameter, Profession &profession) :
        element(Element_Type::other, profession.get_name(), profession, source_mapping::Source_Range()),
        parameter(parameter),
        node(element, parameter.get_node().get_dungeon(), parameter.get_node().get_function()) {}

      Common_Element & get_element(){
        return element;
      }

      const Common_Element & get_element() const{
        return element;
      }
  };

  using Generic_Argument_Owner = std::unique_ptr<Generic_Argument>;
  using Generic_Argument_Array = std::vector<Generic_Argument_Owner>;
}