#pragma once

#include <overworld/schema/professions/Profession.h>
#include <overworld/schema/Minion.h>

namespace lifetime {

  enum class Element_Type {
//      none,
//      profession_reference,
        node,
      variable,
  };

  class Overworld_Element {
      Element_Type type;

  public:
//      Overworld_Element() :
//        none(nullptr), type(Element_Type::none) {}

      Overworld_Element(overworld::Node &node) : node(&node), type(Element_Type::node) {}
//      explicit Overworld_Element(overworld::Profession_Reference &profession_reference) :
//        profession_reference(&profession_reference), type(Element_Type::profession_reference) {}

      explicit Overworld_Element(overworld::Variable &variable) : variable(&variable) {}

      union {
          overworld::Node *node;
          overworld::Variable *variable;
//          void *none;
      };

      overworld::Ownership get_ownership() const {
        return overworld::Ownership::unknown;
      }

      void set_ownership(overworld::Ownership) {

      }

      overworld::Element &get_element();
      source_mapping::Source_Range get_source_point();

  };
}