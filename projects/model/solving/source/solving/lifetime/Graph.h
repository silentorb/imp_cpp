#pragma once

#include "Node.h"
#include <vector>

namespace lifetime {

  using Nodes = std::vector<Node_Owner>;

  class Graph {
      Nodes nodes;
      Owned_Connections connections;

  public:
      void connect(Node &first, Node &second, Connection_Type type) {
        auto existing_connection = first.get_connection(second);
        if (existing_connection)
          return;

        auto connection = Connection(first, second, type);
        connections.push_back(connection);
        first.add_connection(connection);
        second.add_connection(connection);

      }

      void add_node(Node_Owner node) {
        nodes.push_back(std::move(node));
      }
  };
}