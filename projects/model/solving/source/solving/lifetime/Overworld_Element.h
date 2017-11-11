#pragma once

#include <overworld/schema/professions/Profession.h>
#include <overworld/schema/Minion.h>

namespace lifetime {

  enum class Element_Type {
      none,
      profession_reference,
      variable,
  };

  class Overworld_Element {
      Element_Type type;

  public:
      Overworld_Element() :
        none(nullptr), type(Element_Type::none) {}

      explicit Overworld_Element(const overworld::Profession_Reference &profession_reference) :
        profession_reference(&profession_reference), type(Element_Type::profession_reference) {}

      explicit Overworld_Element(overworld::Variable &variable) : variable(&variable) {}

      union {
          const overworld::Profession_Reference *profession_reference;
          const overworld::Variable *variable;
          const void *none;
      };

      overworld::Ownership get_ownership() const {
        return overworld::Ownership::unknown;
      }

      void set_ownership(overworld::Ownership) {

      }

  };
}