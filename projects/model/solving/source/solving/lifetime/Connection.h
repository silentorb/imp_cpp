#pragma once

#include <vector>

namespace lifetime {

  enum Connection_Type {

  };

  class Node;

  class Connection {
      Connection_Type type;
      Node *nodes[2];

  public:
      Connection(Node &first, Node &second, Connection_Type type) :
        type(type) {
        nodes[0] = &first;
        nodes[1] = &second;
      }

      Node &get_first() {
        return *nodes[0];
      }

      Node &get_second() {
        return *nodes[1];
      }

      const Node &get_first() const {
        return *nodes[0];
      }

      const Node &get_second() const {
        return *nodes[1];
      }

      Node &get_other(const Node &node) {
        return &node == nodes[1]
               ? *nodes[0] : *nodes[1];
      }
  };

  using Owned_Connections = std::vector<Connection>
}