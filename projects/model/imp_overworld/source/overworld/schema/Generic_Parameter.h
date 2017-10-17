#pragma once

#include <memory>
#include <overworld/imp_graph/Node.h>
#include "Member.h"
#include "Element.h"

namespace overworld {

  class Function_Interface;

//  using Generic_Parameter_Node = Node;
//  class Generic_Parameter_Node : public Node {
//      Element &element;
//
//  public:
//      Generic_Parameter_Node(Element &element, Dungeon_Interface *dungeon,
//                             Function_Interface *function) :
//        Node(element.get_profession(), dungeon, function), element(element) {}
//
//      Node_Status get_status() const override {
//        return Node_Status::unresolved;
//      }
//
//      std::string get_debug_string() const override {
//        return "GP " + Node::get_debug_string();
//      }
//
//      Element &get_element() override {
//        return element;
//      }
//
//      const Element &get_element() const override {
//        return element;
//      }
//
//  };

  class Generic_Parameter_Element : public Common_Element {
  public:
      Generic_Parameter_Element(Element_Type type, const std::string &name, Profession_Reference profession,
                                const source_mapping::Source_Range &source_point) :
        Common_Element(type, name, nullptr, nullptr, source_point) {}

//      void set_profession(Profession_Reference &value, Profession_Setter &setter) override {
//        throw std::runtime_error("Not supported.");
//      }

      void set_name(const std::string &value) {
        name = value;
      }
  };

  class Generic_Parameter : public Profession {
      Generic_Parameter_Element element;
      Node node;

  public:
      Generic_Parameter(const std::string &name, Profession_Reference &profession, Dungeon_Interface *dungeon,
                        Function_Interface *function,
                        const source_mapping::Source_Range &source_point) :
//        element(Element_Type::other, name, *this, source_point),
        element(Element_Type::other, name, profession, source_point),
        node(profession, element) {
        throw std::runtime_error("Not implemented");
      }

      ~Generic_Parameter() override = default;

      const std::string get_name() const override {
        return element.get_name();
      }

      void set_name(const std::string &value) {
        element.set_name(value);
      }

      Node &get_node() override {
        return node;
      }

      const Node &get_node() const {
        return node;
      }

      Profession_Type get_type() const override {
        return Profession_Type::generic_parameter;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      const Scope *get_scope() const override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }

      Ownership get_ownership() const override {
        return Ownership::owner;
      }

      Profession_Reference &get_base(Profession_Reference &self) override {
        return self;
      }

      const Profession &get_base() const override {
        return *this;
      }

      Generic_Parameter_Element &get_element() {
        return element;
      }
  };

  using Generic_Parameter_Owner = std::unique_ptr<Generic_Parameter>;
  using Generic_Parameter_Array = std::vector<Generic_Parameter *>;

  Owned_Profession_Reference<Generic_Parameter> &add_generic_parameter_to_vector(
    std::vector<Owned_Profession_Reference<Generic_Parameter>> &generic_parameters,
    Dungeon_Interface *dungeon, Function_Interface *function);

//  void rename_generic_parameters(std::vector<Generic_Parameter_Owner> &generic_parameters);
  std::vector<Profession_Reference> to_professions(const Generic_Parameter_Array &generic_parameters,
                                                   size_t additional_space = 0);

  std::string get_generic_parameter_name(size_t index);

}