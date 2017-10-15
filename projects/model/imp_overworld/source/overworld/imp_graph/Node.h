#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/professions/Profession_Reference.h>
#include <overworld/schema/Dungeon_Interface.h>
#include <overworld/schema/Element.h>

namespace overworld {

  class Function_Interface;

  enum class Node_Status {
      unresolved,
      resolved,
      partial,
  };

  class Node : public graphing::Node<Node, Connection> {
      Node_Status status = Node_Status::unresolved;
      bool changed = false;
      Dungeon_Interface *dungeon = nullptr;
      Function_Interface *function = nullptr;
      Profession_Reference original_profession;

  public:
      Node(Profession_Reference &original_profession, Dungeon_Interface *dungeon, Function_Interface *function) :
        original_profession(original_profession), dungeon(dungeon), function(function) {}

      virtual ~Node() {}

      virtual Element &get_element() = 0;
      virtual const Element &get_element() const = 0;

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
        return dungeon;
      }

      Function_Interface *get_function() const {
        return function;
      }

//      void set_original_profession(Profession &value) {
//        original_profession = &value;
//      }
//
//      Profession &get_original_profession() {
//        return *original_profession;
//      }
  };

  class Element_Reference_Node : public Node {
  protected:
      Element &element;

  public:
      Element_Reference_Node(Element &element, Dungeon_Interface *dungeon,
                             Function_Interface *function) :
        Node(element.get_profession(), dungeon, function), element(element) {}

      Node_Status get_status() const override;

      Element &get_element() override {
        return element;
      }

      const Element &get_element() const override {
        return element;
      }

  };

  class Element_Owner_Node : public Node {
  protected:
      Element_Owner element;

  public:
      Element_Owner_Node(Element_Owner element, Dungeon_Interface *dungeon,
                         Function_Interface *function) :
        Node(element->get_profession(), dungeon, function),
        element(std::move((element))) {}

  };

  class Node_Copy : public Node {
      Node &original;
      Profession_Reference profession;

  public:
      Node_Copy(Node &original, Profession_Reference &profession, Dungeon_Interface *dungeon, Function_Interface *function) :
        Node(profession, dungeon, function), original(original), profession(profession) {}

      Node_Status get_status() const override {
        return profession->get_base().get_type() != overworld::Profession_Type::unknown
               ? Node_Status::resolved
               : Node_Status::unresolved;
      }

      Element &get_element() override {
        return original.get_element();
      }

      Node &get_original() const {
        return original;
      }

      const Element &get_element() const override {
        return original.get_element();
      }
  };

  using Node_Owner = std::unique_ptr<Node>;
}