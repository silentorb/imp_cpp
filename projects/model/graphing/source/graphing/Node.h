#pragma once

#include <memory>
#include <vector>
#include "Abstract_Graph.h"

namespace graphing {

  template<typename N, typename Connection>
  class Node {
      std::vector<Connection *> connections;
      std::vector<N *> nodes;

      Abstract_Graph<N, Connection> *graph = nullptr;

  public:
      virtual ~Node() {
        if (graph) {
          graph->remove(*static_cast<N *>(this));
        }
      }

      std::vector<Connection *> &get_connections() {
        return connections;
      }

      const std::vector<Connection *> &get_connections() const {
        return connections;
      }

      std::vector<N *> &get_neighbors() {
        return nodes;
      }

      void add_connection(Connection &connection) {
        connections.push_back(&connection);
        nodes.push_back(&connection.get_other(*static_cast<N *>(this)));
      }

      Connection *get_connection(N &other) {
        for (auto connection: connections) {
          if (&connection->get_other(*static_cast<N *>(this)) == &other)
            return connection;
        }

        return nullptr;
      }

      void connect(N &other) {
        graph->connect(*static_cast<N *>(this), other);
      }

      bool is_connected(N &other) {
        for (auto node : nodes) {
          if (node == &other)
            return true;
        }

        return false;
      }

      void disconnect(N &other) {
        graph->disconnect(*static_cast<N *>(this), other);
      }

      void replace_with(N &other) {
          for (auto i = nodes.size(); i > 0; --i) {
            auto node = nodes[i - 1];
            disconnect(*node);
            node->connect(other);
          }
      }

      void set_graph(Abstract_Graph<N, Connection> *value) {
        graph = value;
      }

      Abstract_Graph<N, Connection> *get_graph() {
        return graph;
      };
  };

}