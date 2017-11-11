#pragma once

#include <memory>
#include <vector>
#include "Connection.h"

namespace lifetime {

  class Node {
      std::vector<Connection *> connections;
      std::vector<Node *> nodes;

  public:
      std::vector<Connection *> &get_connections() {
        return connections;
      }

      const std::vector<Connection *> &get_connections() const {
        return connections;
      }

      std::vector<Node *> &get_neighbors() {
        return nodes;
      }

      const std::vector<Node *> &get_neighbors() const {
        return nodes;
      }

      void add_connection(Connection &connection) {
        connections.push_back(&connection);
        nodes.push_back(&connection.get_other(*this));
      }

      Connection *get_connection(Node &other) {
        for (auto connection: connections) {
          if (&connection->get_other(*this) == &other)
            return connection;
        }

        return nullptr;
      }

      bool is_connected(Node &other) {
        for (auto node : nodes) {
          if (node == &other)
            return true;
        }

        return false;
      }

  };

  using Node_Owner = std::unique_ptr<Node>;
}