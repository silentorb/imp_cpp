#pragma once

#include <overworld/schema/Minion.h>

namespace overworld {

  class Profession_Library;

  class Generic_Argument_Element : public Common_Element {
      Minion &member;
      Profession_Library &profession_library;
      Node &node;

  public:
      Generic_Argument_Element(Minion &member, Profession &profession, Profession_Library &profession_library, Node &node,
                       const source_mapping::Source_Range &source_point) :
        Common_Element(Element_Type::other, member.get_element().get_name(), profession, source_point),
        member(member),
        profession_library(profession_library),
        node(node) {}

      void set_profession(Profession &value, Profession_Setter &setter) override;

      Minion &get_member() const {
        return member;
      }
  };

  class Generic_Argument_Node : public Node {
      Generic_Argument_Element element;

  public:
      Generic_Argument_Node(Profession &profession, Minion &member_container,
                    Function_Interface *function, Profession_Library &profession_library,
                    const source_mapping::Source_Range &source_point) :
        Node(profession, nullptr, function),
        element(member_container, profession, profession_library, *this, source_point) {}

      bool is_resolved() const override {
        auto &profession = element.get_profession();
        if (profession.get_type() == Profession_Type::generic_parameter)
          return false;

        return profession.get_base().get_type() != overworld::Profession_Type::unknown
               && profession.get_base().get_type() != overworld::Profession_Type::generic_parameter;
      }

      Element &get_element() override {
        return element;
      }

      const Element &get_element() const override {
        return element;
      }

  };

}