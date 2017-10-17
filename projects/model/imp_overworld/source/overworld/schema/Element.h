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

  class Dungeon_Interface;
  class Function_Interface;

  class Element {
  public:
      virtual const source_mapping::Source_Range &get_source_point() const = 0;
      virtual const std::string get_name() const = 0;
      virtual Element_Type get_type() const = 0;
      virtual Dungeon_Interface *get_dungeon() const = 0;
      virtual Function_Interface *get_function() const = 0;
  };

  class Common_Element : public Element {
  protected:
      Element_Type type;
      std::string name;
      source_mapping::Source_Range source_point;
      Dungeon_Interface *dungeon = nullptr;
      Function_Interface *function = nullptr;

  public:
      Common_Element(Element_Type type, const std::string name,
                     Dungeon_Interface *dungeon,
                     Function_Interface *function,
                     const source_mapping::Source_Range &source_point) :
        type(type), name(name), source_point(source_point) {}

      const source_mapping::Source_Range &get_source_point() const override {
        return source_point;
      }

      const std::string get_name() const override {
        return name;
      }

      Element_Type get_type() const override {
        return type;
      }

      Dungeon_Interface *get_dungeon() const override {
        return dungeon;
      }

      Function_Interface *get_function() const override {
        return function;
      }
  };

  using Element_Owner = std::unique_ptr<Element>;

//  class Empty_Profession_Setter : public overworld::Profession_Setter {
//  public:
//      static Empty_Profession_Setter &get_instance();
//      void set_profession(overworld::Node &node, overworld::Profession_Reference &profession) override;
//  };

}