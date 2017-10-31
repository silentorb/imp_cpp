#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/professions/Profession_Reference.h>
#include <overworld/schema/Element.h>

namespace overworld {

  class Function_Interface;

  class Basic_Dungeon;

  enum class Node_Status {
      unresolved,
      resolved,
      partial,
  };

  class Node;

  Node_Status get_status_using_profession(const Profession &profession);

  class Profession_Setter {
  public:
      virtual void set_profession(Node &node, Profession_Reference &profession) = 0;
  };

  class Node : public graphing::Node<Node, Connection> {
      Node_Status status = Node_Status::unresolved;
      bool changed = false;
      Profession_Reference original_profession;
      Profession_Reference profession;
      Element &element;

  protected:

  public:
      Node(Profession_Reference original_profession, Element &element) :
        element(element),
        original_profession(original_profession),
        profession(original_profession) {
        status = _get_status();
      }

      virtual Node_Status _get_status() const {
        auto &base_profession = profession->get_base();
        return get_status_using_profession(base_profession);
      }

      Node(const Node &) = delete;

      virtual ~Node() {}

      Element &get_element() {
        return element;
      }

      const Element &get_element() const {
        return element;
      }

      Node_Status get_status() const {
        return status;
      }

      Parent &get_parent() {
        return element.get_parent();
      }

//      void set_status(Node_Status value) {
//        status = value;
//      }

      bool is_changed() const {
        return changed;
      }

      void set_changed(bool changed) {
        Node::changed = changed;
      }

      virtual std::string get_debug_string() const;

      Profession_Reference &get_profession() {
        return profession;
      }

      const Profession &get_profession() const {
        return *profession;
      }

      void set_profession(Profession_Reference &value, Profession_Setter &setter) {
        profession = value;
        status = _get_status();
      }

  };

  class Empty_Profession_Setter : public overworld::Profession_Setter {
  public:
      static Empty_Profession_Setter &get_instance();
      void set_profession(overworld::Node &node, overworld::Profession_Reference &profession) override;
  };

//  class Element_Reference_Node : public Node {
//  protected:
//      Element &element;
//
//  public:
//      Element_Reference_Node(Element &element, Dungeon_Interface *dungeon,
//                             Function_Interface *function) :
//        Node(element.get_profession(), dungeon, function), element(element) {}
//
//      Node_Status get_status() const override;
//
//      Element &get_element() override {
//        return element;
//      }
//
//      const Element &get_element() const override {
//        return element;
//      }
//
//  };

//  class Element_Owner_Node : public Node {
//  protected:
//      Element_Owner element;
//
//  public:
//      Element_Owner_Node(Element_Owner element, Dungeon_Interface *dungeon,
//                         Function_Interface *function) :
//        Node(element->get_profession(), dungeon, function),
//        element(std::move((element))) {}
//
//  };

  class Node_Copy : public Node {
      Node &original;
      Profession_Reference profession;

  protected:

  public:
      Node_Copy(Node &original, Profession_Reference &profession, Basic_Dungeon *dungeon,
                Function_Interface *function) :
        Node(profession, original.get_element()), original(original), profession(profession) {}

      Node_Status _get_status() const override {
        return profession->get_base().get_type() != overworld::Profession_Type::unknown
               ? Node_Status::resolved
               : Node_Status::unresolved;
      }

      Node &get_original() const {
        return original;
      }
  };

  using Node_Owner = std::unique_ptr<Node>;
}