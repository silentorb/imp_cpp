#pragma once

#include <memory>
#include <vector>

namespace graphing {

  template <typename Connection>
  class Node_Modifier;

  template <typename Connection>
  class Node {
      std::vector<Connection *> connections;

      template<typename> friend class Node_Modifier;

  public:
      std::vector<Connection *> &get_connections() {
        return connections;
      }

//      void each_neighbor(const std::function<void(Node &)> &handler) {
//        for (auto connection: connections) {
//          auto &node = &connection->get_first() != this
//                       ? connection->get_first()
//                       : connection->get_second();
//
//          handler(node);
//        }
//      }
  };

  // Nodes should only be modified through big-picture classes like Graph.
  // To minimize foot shooting, all node modification functions are not
  // immediately accessible.
  template <typename Connection>
  class Node_Modifier {
  public:

      static void add_connection(Node<Connection> &node, Connection &connection) {
        node.connections.push_back(&connection);
      }
  };

  }