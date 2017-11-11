#pragma once

#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Variable {
  protected:
      Common_Element element;
      Node node;

  public:
      Variable(Element_Type element_type, const std::string &name, Profession_Reference &profession,
               Parent parent, const source_mapping::Source_Range &source_range) :
        element(element_type, name, parent, source_range),
        node(profession, element) {}

      Common_Element &get_element() {
        return element;
      }

      Node &get_node() {
        return node;
      }

      void set_profession(Profession_Reference &profession, Profession_Setter &setter) {
        node.set_profession(profession, setter);
      }

      const Profession_Reference &get_profession() const {
        return node.get_profession();
      }

      const Profession &get_simple_profession() const {
        return node.get_simple_profession();
      }

      Profession_Reference &get_profession() {
        return node.get_profession();
      }

      const std::string get_name() const {
        return element.get_name();
      }
  };
}