#pragma once

#include "Parameter.h"
#include "professions/Profession.h"

namespace overworld {

  class Function_Signature {
      std::vector<Parameter_Owner> parameters;
      Node &node;
      Profession *return_type = nullptr;

  public:
      Function_Signature(Node &node, Profession *return_type = nullptr) :
        node(node), return_type(return_type) {}

      const std::vector<Parameter_Owner> &get_parameters() const {
        return parameters;
      }

      Profession *get_return_type() const {
        return return_type;
      }

      void add_parameter(Parameter_Owner parameter) {
        parameters.push_back(std::move(parameter));
      }

      void set_return_type(Profession &profession) {
        return_type = &profession;
      }

      Node &get_node() {
        return node;
      }
  };


  class Function_Profession : public Profession {
      Common_Element element;
      Element_Reference_Node node;
      std::vector<Simple_Parameter_Owner> elements;

  public:
      Function_Profession(Dungeon_Interface *dungeon, Function_Interface *function,
                          const source_mapping::Source_Range &source_range) :
        element(Element_Type::other, "Function Signature", *this, source_range),
        node(element, dungeon, function) {}

      ~Function_Profession() override = default;

      Profession_Type get_type() const override {
        return Profession_Type::function;
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
        return Ownership::unknown;
      }

      const std::string get_name() const override {
        return "Function Profession";
      }

      const std::string get_debug_name() const override {
        return "Function Profession";
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }

      void add_element(Simple_Parameter_Owner element) {
        elements.push_back(std::move(element));
      }

      const std::vector<Simple_Parameter_Owner> &get_elements() const {
        return elements;
      }
  };

}