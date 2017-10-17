#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/professions/Profession_Reference.h>
#include <overworld/schema/Element.h>

namespace overworld {

  class Function_Interface;
  class Dungeon_Interface;

  enum class Node_Status {
      unresolved,
      resolved,
      partial,
  };

  class Node;

  class Profession_Setter {
  public:
      virtual void set_profession(Node &node, Profession_Reference &profession) = 0;
  };

  class Node : public graphing::Node<Node, Connection> {
      Node_Status status = Node_Status::unresolved;
      bool changed = false;
//      Dungeon_Interface *dungeon = nullptr;
//      Function_Interface *function = nullptr;
      Profession_Reference original_profession;
      Profession_Reference profession;
      Element &element;

  public:
      Node(Profession_Reference &original_profession, Element &element) :
        element(element),
        original_profession(original_profession),
        profession(profession) {}

//      virtual ~Node() {}
//
       Element &get_element() {
        return element;
      }
       const Element &get_element() const {
         return element;
       }

      virtual Node_Status get_status() const {
        return status;
      }

      void set_status(Node_Status value) {
        status = value;
      }

      bool is_changed() const {
        return changed;
      }

      void set_changed(bool changed) {
        Node::changed = changed;
      }

      virtual std::string get_debug_string() const;

      Dungeon_Interface *get_dungeon() const {
        return element.get_dungeon();
      }

      Function_Interface *get_function() const {
        return element.get_function();
      }

//      virtual Profession_Reference &get_profession() = 0;
//      virtual const Profession_Reference &get_profession() const = 0;
//      virtual void set_profession(Profession_Reference &value, Profession_Setter &setter) = 0;

      Profession_Reference &get_profession() {
        return profession;
      }

      const Profession_Reference &get_profession() const {
        return profession;
      }

      void set_profession(Profession_Reference &value, Profession_Setter &setter) {
        profession = value;
      }

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

//  class Node_Copy : public Node {
//      Node &original;
//      Profession_Reference profession;
//
//  public:
//      Node_Copy(Node &original, Profession_Reference &profession, Dungeon_Interface *dungeon,
//                Function_Interface *function) :
//        Node(profession, dungeon, function), original(original), profession(profession) {}
//
//      Node_Status get_status() const override {
//        return profession->get_base().get_type() != overworld::Profession_Type::unknown
//               ? Node_Status::resolved
//               : Node_Status::unresolved;
//      }
//
//      Element &get_element() override {
//        return original.get_element();
//      }
//
//      Node &get_original() const {
//        return original;
//      }
//
//      const Element &get_element() const override {
//        return original.get_element();
//      }
//  };

  using Node_Owner = std::unique_ptr<Node>;
}