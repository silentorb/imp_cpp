#pragma once

#include <vector>
#include <memory>
#include "Abstract_Graph.h"
#include <functional>

namespace graphing {

  template<typename Node, typename Connection>
  class Reference_Graph : public Abstract_Graph<Node, Connection> {
  public:
      using Node_Delegate = std::function<void(Node &)>;
      using Connection_Delegate = std::function<void(Connection &)>;

  private:
      using Connection_Owner = std::unique_ptr<Connection>;

      std::vector<Node *> nodes;
      std::vector<Connection_Owner> connections;
      Node_Delegate on_add;
      Node_Delegate on_remove;
      Connection_Delegate on_connect;

      void _disconnect(Node &first, Node &second) {
        auto &first_nodes = first.get_neighbors();
        for (auto i = first_nodes.begin(); i != first_nodes.end(); i++) {
          if (*i == &second) {
            first_nodes.erase(i);
            goto next;
          }
        }
        throw std::runtime_error("Could not find node.");

        next:
        auto &node_connections = first.get_connections();
        for (auto i = node_connections.begin(); i != node_connections.end(); i++) {
          if (&(*i)->get_other(first) == &second) {
            node_connections.erase(i);
            return;
          }
        }

        throw std::runtime_error("Could not find connection.");
      }

      bool has_node(Node &node) {
        for (auto &pointer : nodes) {
          if (pointer == &node) {
            return true;
          }
        }
        return false;
      }

      Connection &_connect(Node &first, Node &second) {
        auto existing_connection = first.get_connection(second);
        if (existing_connection)
          return *existing_connection;

        auto connection = new Connection(first, second);
        connections.push_back(std::unique_ptr<Connection>(connection));
        first.add_connection(*connection);
        second.add_connection(*connection);
        if (on_connect)
          on_connect(*connection);

        return *connection;
      }

  public:
      virtual ~Reference_Graph() {
        for (auto node: nodes) {
          node->set_graph(nullptr);
        }
      }

      const std::vector<Node *> &get_nodes() const {
        return nodes;
      }

      const std::vector<Connection_Owner> &get_connections() const {
        return connections;
      }

      void add_node(Node &node) {
        nodes.push_back(&node);
        node.set_graph(this);
        if (on_add)
          on_add(node);

        for (auto other : node.get_neighbors()) {
          if (!has_node(*other))
            add_node(*other);
        }
      }

      Connection &connect(Node &first, Node &second) override {
        if (!has_node(first))
          add_node(first);

        if (!has_node(second))
          add_node(second);

        return _connect(first, second);
      }

      void connect(Node &first, Node &second, std::unique_ptr<Connection> connection) {
        if (!has_node(first))
          add_node(first);

        if (!has_node(second))
          add_node(second);

        auto &connection_reference = *connection;
        connections.push_back(std::move(connection));
        first.add_connection(connection_reference);
        second.add_connection(connection_reference);
        if (on_connect)
          on_connect(connection_reference);
      }

      Connection &connect_without_adding(Node &first, Node &second) {
        return _connect(first, second);
      }

      void disconnect(Node &first, Node &second) override {
        auto connection = first.get_connection(second);
        _disconnect(first, second);
        _disconnect(second, first);

        for (auto i = connections.begin(); i != connections.end(); i++) {
          if ((*i).get() == connection) {
            connections.erase(i);
            break;
          }
        }

        if (first.is_connected(second))
          throw std::runtime_error("Incomplete disconnect.");

        if (second.is_connected(first))
          throw std::runtime_error("Incomplete disconnect.");
      }

      void remove(Node &node) override {
        if (on_remove)
          on_remove(node);

        auto &neighbors = node.get_neighbors();
        for (auto i = neighbors.size(); i > 0; --i) {
          neighbors[i - 1]->disconnect(node);
        }

        for (auto i = nodes.begin(); i != nodes.end(); i++) {
          if (*i == &node) {
            nodes.erase(i);
            break;
          }
        }
      }

      void set_on_add(const Node_Delegate &value) {
        on_add = value;
      }

      void set_on_remove(const Node_Delegate &value) {
        on_remove = value;
      }

      void set_on_connect(const Connection_Delegate &value) {
        on_connect = value;
      }
  };
}