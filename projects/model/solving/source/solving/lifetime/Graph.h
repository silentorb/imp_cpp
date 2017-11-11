#pragma once

#include "Node.h"
#include "Context.h"
#include <vector>
#include <unordered_map>

namespace lifetime {

  using Nodes = std::vector<Node_Owner>;

  class Graph {
      Nodes nodes;
      Owned_Connections connections;
      std::unordered_map<overworld::Variable *, Node *> variable_nodes;

  public:
      void connect(Node &first, Node &second, Connection_Type type = Connection_Type::direct) {
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

      Nodes &get_nodes() {
        return nodes;
      }

      Owned_Connections &getConnections() {
        return connections;
      }

      Node &get_or_create_variable_node(overworld::Variable &variable) {
        if (variable_nodes.count(&variable))
          return *variable_nodes[&variable];

        auto node = new Node(Overworld_Element(variable), Lifetime_Ownership::unknown);
        add_node(Node_Owner(node));
        return *node;
      }
  };
}