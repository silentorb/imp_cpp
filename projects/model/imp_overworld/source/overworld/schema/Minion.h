#pragma once

#include <memory>
#include "Member.h"
#include "overworld/schema/professions/Profession.h"
#include <underworld/schema/Minion.h>
#include <overworld/imp_graph/Node.h>
#include <overworld/schema/professions/Profession_Reference.h>

namespace overworld {

  class Dungeon;

  class Minion : public virtual Profession_Reference {
      Profession *profession;
      Profession_Node<Minion> node;
      const std::string name;
      const source_mapping::Source_Point source_point;

  public:
      Minion(const std::string &name, Profession &profession, Dungeon_Interface *dungeon,
             const source_mapping::Source_Point &source_point, Function_Interface *function) :
        profession(&profession), node(*this, profession, dungeon, function), name(name),
        source_point(source_point) {}

      virtual ~Minion() {

      }

      const std::string get_name() const override {
        return name;
      }

      const Profession &get_profession() const override {
        return *profession;
      }

      Profession &get_profession() override {
        return *profession;
      }

      virtual void set_profession(Profession &value) override {
        profession = &value;
      }

      virtual const source_mapping::Source_Point &get_source_point() const override {
        return source_point;
      }

      Node &get_node() {
        return node;
      }

      virtual bool is_parameter() const {
        return false;
      }

      Dungeon *get_dungeon() {
        return (Dungeon *) node.get_dungeon();
      }

      Element_Type get_element_type() const override {
        return Element_Type::minion;
      }
  };

  using Minion_Owner = std::unique_ptr<Minion>;

  class Member_Minion : public Generic_Member_Reference<Minion> {
  public:
      Member_Minion(Minion &value) : Generic_Member_Reference(value) {}

      Member_Type get_member_type() const override {
        return Member_Type::minion;
      }

      Minion & get_minion(){
        return value;
      }
  };
}