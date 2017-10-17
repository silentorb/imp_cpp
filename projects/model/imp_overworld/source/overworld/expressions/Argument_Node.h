#pragma once

#include <overworld/schema/Minion.h>

namespace overworld {

  class Profession_Library;

  class Argument_Element : public Common_Element {
      Minion &member;
      Profession_Library &profession_library;
      Node &node;

  public:
      Argument_Element(Minion &member, Profession_Reference &profession, Profession_Library &profession_library,
                       Node &node,
                       const source_mapping::Source_Range &source_point) :
        Common_Element(Element_Type::other, member.get_element().get_name(), nullptr, nullptr, source_point),
        member(member),
        profession_library(profession_library),
        node(node) {}

//      void set_profession(Profession &value, Profession_Setter &setter) override;

      Minion &get_member() const {
        return member;
      }
  };

//  class Argument_Node : public Node {
//      Argument_Element element;
//
//  public:
//      Argument_Node(Profession_Reference &profession, Minion &member_container,
//                    Function_Interface *function, Profession_Library &profession_library,
//                    const source_mapping::Source_Range &source_point) :
//        Node(profession, element),
//        element(member_container, profession, profession_library, *this, source_point) {}
//
//      Node_Status get_status() const override {
//        auto &profession = get_profession();
//        if (profession.get_type() == Profession_Type::generic_parameter)
//          return Node_Status::unresolved;
//
//        auto type = profession.get_base().get_type();
//        return type != overworld::Profession_Type::unknown
//               && type != overworld::Profession_Type::generic_parameter
//               ? Node_Status::resolved
//               : Node_Status::unresolved;
//      }
//
//      std::string get_debug_string() const override {
//        return Node::get_debug_string() + " (Argument)";
//      }
//
//  };

}