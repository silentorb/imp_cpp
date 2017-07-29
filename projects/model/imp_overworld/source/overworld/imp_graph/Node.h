#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"
#include <overworld/schema/professions/Profession_Reference.h>

namespace overworld {

  class Node : public graphing::Node<Connection, Node> {
      bool resolved = false;
      bool changed = false;

  public:
      Node() {

      }

      virtual ~Node() {}

      virtual Profession_Reference &get_profession_reference() = 0;

      virtual void set_profession(Profession &value) {
        get_profession_reference().set_profession(value);
      }

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

//      virtual const std::string get_name() const = 0;
  };

  template<typename T>
  class Profession_Node : public Node {
      T &element;

  public:
      Profession_Node(T &element) : element(element) {}

      Profession_Reference &get_profession_reference() override {
        return element;
      }

//      const std::string get_name() const override {
//        return element.get_name();
//      }
  };

  using Node_Pointer = std::unique_ptr<Node>;
}