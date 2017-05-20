#pragma once

#include <memory>
#include <vector>
#include <graphing/Node.h>
#include "Connection.h"

namespace solving {

  class Node_Modifier;

  class Node : public graphing::Node<Connection> {
      bool resolved;
      bool changed;

      friend class Node_Modifier;

  public:

      bool is_resolved() const {
        return resolved;
      }

      bool is_changed() const {
        return changed;
      }
  };

  // Nodes should only be modified through big-picture classes like Graph.
  // To minimize foot shooting, all node modification functions are not
  // immediately accessible.
  class Node_Modifier {
  public:
      static void set_resolved(Node &node, bool value) {
        node.resolved = value;
      }

      static void set_changed(Node &node, bool value) {
        node.changed = value;
      }
  };

  using Node_Pointer = std::unique_ptr<Node>;
}