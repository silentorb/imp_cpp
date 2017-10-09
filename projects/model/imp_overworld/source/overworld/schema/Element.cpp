#include "Element.h"
#include <overworld/imp_graph/Node.h>

namespace overworld {

  static Empty_Profession_Setter setter;

  Empty_Profession_Setter &Empty_Profession_Setter::get_instance() {
    return setter;
  }

  void Empty_Profession_Setter::set_profession(overworld::Node &node, overworld::Profession &profession) {
    node.get_element().set_profession(profession, *this);
  }
}