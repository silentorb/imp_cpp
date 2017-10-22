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

  class Minion {
      Common_Element element;
      Node node;
      Enchantment_Container enchantments;
      File *header_file = nullptr;

  public:
      Minion(const std::string &name, Profession_Reference &profession, Parent parent,
             const source_mapping::Source_Range &source_point) :
        element(Element_Type::minion, name, parent, source_point),
        node(profession, element) {}

      Minion(const Minion &) = delete;

      virtual ~Minion() {
        int k = 0;
      }

      Common_Element &get_element() {
        return element;
      }

      Node &get_node() {
        return node;
      }

      Enchantment_Container &get_enchantments() {
        return enchantments;
      }

      const Enchantment_Container &get_enchantments() const {
        return enchantments;
      }

      bool has_enchantment(const Dungeon &enchantment) const {
        return enchantments.has_enchantment(enchantment);
      }

      virtual Element_Type get_element_type() const {
        return Element_Type::minion;
      }

      const Profession &get_profession() const {
        return node.get_profession();
      }

      Profession_Reference &get_profession() {
        return node.get_profession();
      }

      const File *get_file() const {
        return header_file;
      }

      void set_file(File *value) {
        header_file = value;
      }
//      virtual void set_profession(Profession &value) {
//        element.set_profession(value);
//      }

      const std::string get_name() const {
        return element.get_name();
      }
  };

  using Minion_Owner = std::unique_ptr<Minion>;
  using Simple_Minion_Owner = std::unique_ptr<Simple_Minion>;
}