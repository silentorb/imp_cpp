#pragma once

#include <memory>
#include <overworld/imp_graph/Node.h>

namespace overworld {

  class Function_Interface;

  class Generic_Parameter : public virtual Profession_Reference, public virtual Profession {
      underworld::Source_Point source_point;
      std::string name;
      Profession_Node <Generic_Parameter> node;

  public:
      Generic_Parameter(const std::string &name, Dungeon_Interface &dungeon, Function_Interface *function)
        : node(*this, *this, dungeon, function), name(name) {}

      virtual ~Generic_Parameter() {}

      Profession &get_profession() override {
        return *this;
      }

      const Profession &get_profession() const override {
        return *this;
      }

      const std::string get_name() const override {
        return name;
      }

      void set_name(const std::string &value) {
        name = value;
      }

      Profession_Node <Generic_Parameter> &get_node() override {
        return node;
      }

      const Profession_Node <Generic_Parameter> &get_node() const {
        return node;
      }

      void set_profession(Profession &value) override {
        throw std::runtime_error("Not supported");
      }

      const underworld::Source_Point &get_source_point() const override {
        return source_point;
      }

      Profession_Type get_type() const override {
        return Profession_Type::generic_parameter;
      }

      Scope *get_scope() override {
        return nullptr;
      }

      File *get_file() const override {
        return nullptr;
      }

      Ownership get_ownership() const override {
        return Ownership::owner;
      }

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }
  };

  using Generic_Parameter_Owner = std::unique_ptr<Generic_Parameter>;
  using Generic_Parameter_Array = std::vector<Generic_Parameter*>;

  Generic_Parameter &add_generic_parameter_to_vector(std::vector<Generic_Parameter_Owner> &generic_parameters,
                                           Dungeon_Interface &dungeon, Function_Interface *function);

  Generic_Parameter &rename_generic_parameters(std::vector<Generic_Parameter_Owner> &generic_parameters);

//  class Contained_Generic_Parameter : public Generic_Parameter {
//
//  public:
//
//
//      virtual ~Contained_Generic_Parameter() {}
//
//
//  };

//  class Generic_Parameter_Reference : public Generic_Parameter {
//      Generic_Parameter *target = nullptr;
//
//  public:
//      Generic_Parameter_Reference(Generic_Parameter &target)
//        : target(&target) {}
//
//      virtual ~Generic_Parameter_Reference() {}
//
//      const std::string get_name() const override {
//        return target->get_name();
//      }
//
//      Profession_Node <Generic_Parameter> &get_node() override {
//        return target->get_node();
//      }
//
//      const Profession_Node <Generic_Parameter> &get_node() const override {
//        return target->get_node();
//      }
//  };

}