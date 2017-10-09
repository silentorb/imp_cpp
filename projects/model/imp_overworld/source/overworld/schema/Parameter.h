#pragma once

#include "Dungeon.h"

namespace overworld {

  class Parameter {
      Common_Element element;
      Element_Reference_Node node;

  public:
      Parameter(const std::string name, Profession &profession, Dungeon_Interface *dungeon,
                Function_Interface *function, const source_mapping::Source_Range &source_range) :
        element(Element_Type::parameter, name, profession, source_range),
        node(element, dungeon, function) {}

      void set_profession(Profession &profession, Profession_Setter &setter) {
        element.set_profession(profession, setter);
      }

      Profession &get_profession() {
        return element.get_profession();
      }

      const Profession &get_profession() const {
        return element.get_profession();
      }

      Element_Reference_Node &get_node() {
        return node;
      }

      const std::string get_name() const {
        return element.get_name();
      }
  };

  using Parameter_Owner = std::unique_ptr<Parameter>;

//  class Parameter : public Minion {
//      std::unique_ptr<Dungeon> temporary_interface;
//
//  public:
//      Parameter(const std::string &name, Profession &profession,
//                const source_mapping::Source_Range &source_point, Function_Interface &function) :
//        Minion(name, profession, nullptr, source_point, &function) {}
//
//      virtual ~Parameter();
//
//      bool is_parameter() const override {
//        return true;;
//      }
//
//      virtual bool transfers_ownership() const {
//        return get_profession().get_ownership() == Ownership::owner;
//      }
//
//      const std::unique_ptr<Dungeon> &get_temporary_interface() const {
//        return temporary_interface;
//      }
//
//      void set_temporary_interface(std::unique_ptr<Dungeon> &value) {
//        temporary_interface = std::move(value);
//      }
//
////      void set_profession(Profession &value, Profession_Setter &setter) override;
//      Dungeon &get_or_create_interface();
//
//      Element_Type get_element_type() const override {
//        return Element_Type::parameter;
//      }
//  };

//  using Parameter_Owner = std::unique_ptr<Parameter>;

}