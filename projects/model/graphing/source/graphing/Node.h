#pragma once

#include <memory>
#include <vector>

namespace graphing {

  template<typename Connection, typename N>
  class Node {
      std::vector<Connection *> connections;
      std::vector<N *> nodes;

  public:
      std::vector<Connection *> &get_connections() {
        return connections;
      }

      const std::vector<Connection *> &get_connections() const {
        return connections;
      }

      std::vector<N *> get_neighbors() {
        return nodes;
      }

      void add_connection(Connection &connection) {
        connections.push_back(&connection);
        nodes.push_back(&connection.get_other(*static_cast<N*>(this)));
      }
  };

}