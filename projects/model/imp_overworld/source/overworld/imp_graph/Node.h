#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/professions.h>

//  class Node_Modifier;

namespace overworld {

  class Node : public graphing::Node<Connection, Node> {
      bool resolved;
      bool changed;

  public:
      virtual Profession_Reference &get_profession_reference() = 0;

      bool is_resolved() const {
        return resolved;
      }

      bool is_changed() const {
        return changed;
      }

      void set_resolved(bool resolved) {
        Node::resolved = resolved;
      }

      void set_changed(bool changed) {
        Node::changed = changed;
      }
  };

  template<typename T>
  class Profession_Node : public Node {
      T &element;

  public:
      Profession_Node(T &element) : element(element) {}

      Profession_Reference &get_profession_reference() override {
        return element;
      }

  };

//  template<typename T>
//  class Mutable_Profession_Node : public Node {
//      T &element;
//
//  public:
//      Mutable_Profession_Node(T &element) : element(element) {}
//
//      const Profession &get_profession() override {
//        return element.get_profession();
//      }
//
//      void set_profession(const Profession &profession) override {
//        element.set_profession(profession);
//      }
//  };

//      friend class Node_Modifier;


//  // Nodes should only be modified through big-picture classes like Graph.
//  // To minimize foot shooting, all node modification functions are not
//  // immediately accessible.
//  class Node_Modifier {
//  public:
//      static void set_resolved(Node &node, bool value) {
//        node.resolved = value;
//      }
//
//      static void set_changed(Node &node, bool value) {
//        node.changed = value;
//      }
//  };

  using Node_Pointer = std::unique_ptr<Node>;
}