#pragma once

#include <memory>
#include <vector>
#include <overworld/schema/professions/Profession.h>
#include "Connection.h"
#include "Context.h"
#include "Overworld_Element.h"

namespace lifetime {

  enum class Lifetime_Ownership {
      unknown,

      anchor,
      copy,
      move,
      reference,
  };

  class Node {
      std::vector<Connection *> connections;
      std::vector<Node *> nodes;
      Lifetime_Ownership ownership;
      Overworld_Element element;

  public:
      Node(const Overworld_Element &element, Lifetime_Ownership ownership) :
        element(element),
        ownership(ownership) {}

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

      Lifetime_Ownership get_ownership() const {
        return ownership;
      }

      void set_ownership(Lifetime_Ownership ownership) {
        Node::ownership = ownership;
      }

       Overworld_Element &get_element() {
        return element;
      }
  };

  using Node_Owner = std::unique_ptr<Node>;
}