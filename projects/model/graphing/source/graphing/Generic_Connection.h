#pragma once

#include <memory>

namespace graphing {

  template<typename Node>
  class Generic_Connection {
      Node *nodes[2];

  public:
      Generic_Connection(Node &first, Node &second) {
        nodes[0] = &first;
        nodes[1] = &second;
      }

      Node *const *get_nodes() {
        return nodes;
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

}