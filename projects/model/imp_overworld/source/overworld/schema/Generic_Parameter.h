#pragma once

#include <memory>
#include <overworld/imp_graph/Node.h>
#include "Member.h"

namespace overworld {

  class Function_Interface;

  template <typename T>
  class Generic_Parameter_Node : public Node {
      T &element;

  public:
      Generic_Parameter_Node(T &parameter, Dungeon_Interface *dungeon,
                             Function_Interface *function) :
        Node(dungeon, function), element(parameter) {}

      Profession_Reference &get_profession_reference() override {
        return element;
      }

      const Profession_Reference &get_profession_reference() const override {
        return element;
      }

      bool is_resolved() const override {
        return false;
      }

      std::string get_debug_string() const override {
        return "GP " + Node::get_debug_string();
      }
  };

  class Generic_Parameter : public virtual Profession_Reference, public virtual Profession {
      source_mapping::Source_Point source_point;
      std::string name;
      Generic_Parameter_Node<Generic_Parameter> node;

  public:
      Generic_Parameter(const std::string &name, Dungeon_Interface *dungeon, Function_Interface *function)
        : node(*this, dungeon, function), name(name) {}

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

      Generic_Parameter_Node<Generic_Parameter> &get_node() override {
        return node;
      }

      const Generic_Parameter_Node<Generic_Parameter> &get_node() const {
        return node;
      }

      void set_profession(Profession &value) override {
        throw std::runtime_error("Not supported");
      }

      const source_mapping::Source_Point &get_source_point() const override {
        return source_point;
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

      Profession &get_base() override {
        return *this;
      }

      const Profession &get_base() const override {
        return *this;
      }
  };

  using Generic_Parameter_Owner = std::unique_ptr<Generic_Parameter>;
  using Generic_Parameter_Array = std::vector<Generic_Parameter *>;

  Generic_Parameter &add_generic_parameter_to_vector(std::vector<Generic_Parameter_Owner> &generic_parameters,
                                                     Dungeon_Interface *dungeon, Function_Interface *function);

//  void rename_generic_parameters(std::vector<Generic_Parameter_Owner> &generic_parameters);
  std::vector<Profession *> to_professions(const Generic_Parameter_Array &generic_parameters,
                                           size_t additional_space = 0);

  std::string get_generic_parameter_name(size_t index);

}