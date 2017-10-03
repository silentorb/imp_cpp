#pragma once

#include <string>
#include <overworld/schema/professions/Profession.h>
#include <source_mapping/Source_Point.h>

namespace overworld {

  enum class Element_Type {
      instantiation,
      minion,
      parameter,
      other
  };

  class Node;

  class Profession_Setter {
  public:
      virtual void set_profession(Node &node, Profession &profession) = 0;
  };

  class Element {
  public:
      virtual Profession &get_profession() = 0;
      virtual const Profession &get_profession() const = 0;
      virtual void set_profession(Profession &value, Profession_Setter &setter) = 0;
      virtual const source_mapping::Source_Range &get_source_point() const = 0;
      virtual const std::string get_name() const = 0;
      virtual Element_Type get_type() const = 0;
  };

  class Common_Element : public Element {
  protected:
      Element_Type type;
      std::string name;
      Profession *profession;
      source_mapping::Source_Range source_point;

  public:
      Common_Element(Element_Type type, const std::string name, Profession &profession,
                     const source_mapping::Source_Range &source_point) :
        type(type), name(name), profession(&profession), source_point(source_point) {}

      Profession &get_profession() override {
        return *profession;
      }

      const Profession &get_profession() const override {
        return *profession;
      }

      void set_profession(Profession &value, Profession_Setter &setter) override {
        profession = &value;
      }

      const source_mapping::Source_Range &get_source_point() const override {
        return source_point;
      }

      const std::string get_name() const override {
        return name;
      }

      Element_Type get_type() const override {
        return type;
      }
  };

  using Element_Owner = std::unique_ptr<Element>;
}