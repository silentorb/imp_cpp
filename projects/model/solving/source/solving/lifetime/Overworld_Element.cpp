#include "Overworld_Element.h"

namespace lifetime {

  overworld::Element &Overworld_Element::get_element() {
    switch (type) {
      case Element_Type::variable:
        return variable->get_element();

      case Element_Type::node:
        return node->get_element();
    }
  }

  source_mapping::Source_Range Overworld_Element::get_source_point() {
    return get_element().get_source_point();
  }
}