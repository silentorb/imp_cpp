#pragma once

#include <memory>
#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Function_Interface;

  class Generic_Parameter : public virtual Profession_Reference {
      Profession *profession;
      Profession_Node <Generic_Parameter> node;
      underworld::Source_Point source_point;

  public:
      Generic_Parameter(Profession &profession, Dungeon_Interface &dungeon, Function_Interface *function) :
        profession(&profession), node(*this, profession, dungeon, function) {}

      Profession &get_profession() override {
        return *profession;
      }

      const Profession &get_profession() const override {
        return *profession;
      }

      void set_profession(Profession &value) override {
        profession = &value;
      }

      const underworld::Source_Point &get_source_point() const override {
        return source_point;
      }

      const std::string get_name() const override {
        return "generic_parameter<" + profession->get_name() + ">";
      }

      Profession_Node <Generic_Parameter> &get_node() {
        return node;
      }

      const Profession_Node <Generic_Parameter> &get_node() const {
        return node;
      }
  };

  using Generic_Parameter_Owner = std::unique_ptr<Generic_Parameter>;
  using Generic_Parameter_Array = std::vector<Generic_Parameter_Owner>;
}