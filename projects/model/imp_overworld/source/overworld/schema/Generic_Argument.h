#pragma once

#include "Generic_Parameter.h"

namespace overworld {

  class Generic_Argument {
      Common_Element element;
      Generic_Parameter &parameter;
      Node node;

  public:
      Generic_Argument(Generic_Parameter &parameter, const Profession_Reference &profession) :
        element(Element_Type::other, profession.get_name() + " (Generic Argument)",
                parameter.get_node().get_element().get_parent(),
                parameter.get_element().get_source_point()),
        parameter(parameter),
        node(profession, element) {}

      Common_Element &get_element() {
        return element;
      }

      const Common_Element &get_element() const {
        return element;
      }

      Node &get_node() {
        return node;
      }

      const Node &get_node() const {
        return node;
      }

      const Profession_Reference &get_profession() const {
        return node.get_profession();
      }
  };

  using Generic_Argument_Owner = std::unique_ptr<Generic_Argument>;
  using Generic_Argument_Array = std::vector<Generic_Argument_Owner>;
}