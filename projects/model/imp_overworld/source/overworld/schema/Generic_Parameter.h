#pragma once

#include <memory>
#include <overworld/imp_graph/Node.h>
#include "Member.h"
#include "Element.h"

namespace overworld {

  class Function;

//  using Generic_Parameter_Node = Node;
//  class Generic_Parameter_Node : public Node {
//      Element &element;
//
//  public:
//      Generic_Parameter_Node(Element &element, Dungeon_Interface *dungeon,
//                             Function *function) :
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
                                Parent parent,
                                const source_mapping::Source_Range &source_point) :
        Common_Element(type, name, parent, source_point) {}

      void set_name(const std::string &value) {
        name = value;
      }
  };

  class Generic_Parameter : public Profession {
      Generic_Parameter_Element element;
      Node node;
//      Profession_Reference self;

  public:
      Generic_Parameter(const std::string &name, Profession_Reference &profession,
                        Parent parent,
                        const source_mapping::Source_Range &source_point) :
        element(Element_Type::other, name, profession, parent, source_point),
        node(profession, element) {
//        throw std::runtime_error("Not implemented");
      }

      virtual ~Generic_Parameter() override = default;

      Generic_Parameter(const Generic_Parameter &) = delete;

      const std::string get_name() const override {
        return element.get_name();
      }

//      Profession_Reference &get_reference() {
//        return self;
//      }

      void set_name(const std::string &value) {
        element.set_name(value);
      }

      Node &get_node() {
        return node;
      }

      const Node &get_node() const {
        return node;
      }

      Profession_Type get_type() const override {
        return Profession_Type::generic_parameter;
      }

      Generic_Parameter_Element &get_element() {
        return element;
      }
  };

  using Generic_Parameter_Owner = std::unique_ptr<Generic_Parameter>;
  using Generic_Parameter_Array = std::vector<Generic_Parameter *>;

  std::unique_ptr<Generic_Parameter> &
  add_generic_parameter_to_vector(std::vector<std::unique_ptr<Generic_Parameter>> &generic_parameters, Parent parent);

//  void rename_generic_parameters(std::vector<Generic_Parameter_Owner> &generic_parameters);
  std::vector<Profession_Reference> to_professions(const Generic_Parameter_Array &generic_parameters,
                                                   size_t additional_space = 0);

  std::string get_generic_parameter_name(size_t index);

}