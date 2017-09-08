#pragma once

#include <overworld/schema/Minion.h>

namespace overworld {

  class Profession_Library;

  class Argument_Node : public Profession_Node<Argument_Node>, public Profession_Reference {
      Profession *profession;
      source_mapping::Source_Point source_point;
      Minion &member_container;
      Profession_Library &profession_library;

  public:
      Argument_Node(Profession &original_profession, Minion &member_container,
                    Function_Interface *function, Profession_Library &profession_library) :
        Profession_Node(*this, original_profession, nullptr, function),
        profession(&original_profession), member_container(member_container), profession_library(profession_library) {}

      Profession &get_profession() override {
        return *profession;
      }

      const Profession &get_profession() const override {
        return *profession;
      }

      void set_profession(Profession &value) override;

      const source_mapping::Source_Point &get_source_point() const override {
        return source_point;
      }

      const std::string get_name() const override {
        return "argument_node";
      }

      bool is_resolved() const override {
        if (profession->get_type() == Profession_Type::generic_parameter)
          return false;

        return Profession_Node::is_resolved();
      }
  };

}