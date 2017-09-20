#pragma once

#include <memory>
#include "Member.h"
#include "overworld/schema/professions/Profession.h"
#include "Enchantment.h"
#include "Enchantment_Container.h"
#include <underworld/schema/Minion.h>
#include <overworld/imp_graph/Node.h>
#include <overworld/schema/professions/Profession_Reference.h>

namespace overworld {

  class Dungeon;

  class Simple_Minion {
  protected:
      const std::string name;
      Profession *profession;

  public:
      Simple_Minion(const std::string &name, Profession &profession) :
        name(name), profession(&profession) {}


      const std::string get_name() const {
        return name;
      }

      const Profession &get_profession() const {
        return *profession;
      }

      Profession &get_profession() {
        return *profession;
      }

      virtual void set_profession(Profession &value) {
        profession = &value;
      }
  };

  template<typename T>
  class Generic_Profession_Reference : public virtual Profession_Reference {
      T &minion;

  public:
      Generic_Profession_Reference(T &minion) : minion(minion) {}

      Profession &get_profession() override {
        return minion.get_profession();
      }

      const Profession &get_profession() const override {
        return minion.get_profession();
      }

      void set_profession(Profession &value) override {
        minion.set_profession(value);
      }

      const source_mapping::Source_Point &get_source_point() const override {
        return minion.get_source_point();
      }

      const std::string get_name() const override {
        return minion.get_name();
      }

      Element_Type get_element_type() const override{
        return minion.get_element_type();
      }
  };

  class Minion : public Simple_Minion {
      Generic_Profession_Reference<Minion> profession_reference;
      Profession_Node<Generic_Profession_Reference<Minion>> node;
      const source_mapping::Source_Point source_point;
      Enchantment_Container enchantments;

  public:
      Minion(const std::string &name, Profession &profession, Dungeon_Interface *dungeon,
             const source_mapping::Source_Point &source_point, Function_Interface *function) :
        Simple_Minion(name, profession),
        profession_reference(*this),
        node(profession_reference, profession, dungeon, function),
        source_point(source_point) {}

      Minion(const Minion &) = delete;

      virtual ~Minion() {}

      virtual const source_mapping::Source_Point &get_source_point() const {
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

      Enchantment_Container &get_enchantments() {
        return enchantments;
      }

      const Enchantment_Container &get_enchantments() const {
        return enchantments;
      }

      bool has_enchantment(const Enchantment &enchantment) const {
        return enchantments.has_enchantment(enchantment);
      }

      virtual Element_Type get_element_type() const {
        return Element_Type::minion;
      }

  };

  using Minion_Owner = std::unique_ptr<Minion>;
  using Simple_Minion_Owner = std::unique_ptr<Simple_Minion>;

  class Member_Minion : public Generic_Member_Reference<Minion> {
  public:
      Member_Minion(Minion &value) : Generic_Member_Reference(value) {}

      Member_Type get_member_type() const override {
        return Member_Type::minion;
      }

      Minion &get_minion() {
        return value;
      }
  };
}