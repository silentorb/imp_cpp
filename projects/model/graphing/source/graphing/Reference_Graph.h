#pragma once

#include <vector>
#include <memory>

namespace graphing {

  template<typename Node, typename Connection>
  class Reference_Graph {
      using Connection_Pointer = std::unique_ptr<Connection>;

      std::vector<Node *> nodes;
      std::vector<Connection_Pointer> connections;

  public:
      const std::vector<Node *> &get_nodes() const {
        return nodes;
      }

      const std::vector<Connection_Pointer> &get_connections() const {
        return connections;
      }

      void add_node(Node &node) {
        nodes.push_back(&node);
      }

      Connection &connect(Node &first, Node &second) {
        auto connection = new Connection(first, second);
        connections.push_back(std::unique_ptr<Connection>(connection));
        return *connection;
      }

  };
}