#pragma once

#include <overworld/schema/Minion.h>

namespace overworld {

  class Profession_Library;

  class Argument_Element : public Common_Element {
      Minion &member;
      Profession_Library &profession_library;
      Node &node;

  public:
      Argument_Element(Minion &member, Profession_Library &profession_library, Node &node) :
        Common_Element(Element_Type::other, member.get_element().get_name(), member.get_profession(),
                       source_point),
        member(member),
        profession_library(profession_library),
        node(node) {}

      void set_profession(Profession &value) override;

      Minion &get_member() const {
        return member;
      }
  };

  class Argument_Node : public Element_Reference_Node {
      Argument_Element element;

  public:
      Argument_Node(Profession &original_profession, Minion &member_container,
                    Function_Interface *function, Profession_Library &profession_library,
                    const source_mapping::Source_Range &source_point) :
        Element_Reference_Node(member_container.get_element(), nullptr, function),
        element(member_container, profession_library, *this) {}

      bool is_resolved() const override {
        if (element.get_member().get_profession().get_type() == Profession_Type::generic_parameter)
          return false;

        return Element_Reference_Node::is_resolved();
      }
  };

}