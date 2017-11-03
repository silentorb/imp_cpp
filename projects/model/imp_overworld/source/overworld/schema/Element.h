#pragma once

#include <string>
#include <overworld/schema/professions/Profession.h>
#include <source_mapping/Source_Point.h>
#include "Parent.h"

namespace overworld {

  enum class Element_Type {
      instantiation,
      minion,
      parameter,
      other,
      temporary,
  };

  class Node;

  class Element {
  public:
      virtual const source_mapping::Source_Range &get_source_point() const = 0;
      virtual const std::string get_name() const = 0;
      virtual Element_Type get_type() const = 0;
      virtual Parent &get_parent() = 0;
//      virtual const Parent &get_parent() const = 0;
  };

  class Common_Element : public Element {
  protected:
      Element_Type type;
      std::string name;
      source_mapping::Source_Range source_point;
      Parent parent;

  public:
      Common_Element(Element_Type type, const std::string name,
                     Parent parent,
                     const source_mapping::Source_Range &source_point) :
        type(type), name(name), source_point(source_point), parent(parent) {}

      const source_mapping::Source_Range &get_source_point() const override {
        return source_point;
      }

      const std::string get_name() const override {
        return name;
      }

      Element_Type get_type() const override {
        return type;
      }

      Parent &get_parent() override {
        return parent;
      }
  };

  using Element_Owner = std::unique_ptr<Element>;

}