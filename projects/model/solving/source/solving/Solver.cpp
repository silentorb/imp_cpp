#include "Solver.h"
#include <algorithm>

namespace solving {

  void Solver::add_node(Node *node) {
//    nodes.push_back(std::unique_ptr<Node>(node));
//    if (node->is_changed()) {
//      changed.push_back(node);
//    }
//    else if (!node->is_resolved()) {
//      unresolved.push_back(node);
//    }
  }

//  Connection &Solver::connect(Node &first, Node &second) {
//    auto connection = new Connection(first, second);
//    connections.push_back(std::unique_ptr<Connection>(connection));
//    return *connection;
//  }

  void Solver::resolve_node(Node &node) {
    if (node.is_resolved())
      return;

    unresolved.erase(std::remove(unresolved.begin(), unresolved.end(), &node), unresolved.end());
    Node_Modifier::set_resolved(node, true);
  }

  void Solver::node_changed(Node &node) {
    if (node.is_changed())
      return;

    changed.push_back(&node);
    Node_Modifier::set_changed(node, true);
  }

  void Solver::node_unchanged(Node &node) {
    if (!node.is_changed())
      return;

    changed.erase(std::remove(changed.begin(), changed.end(), &node), changed.end());
    Node_Modifier::set_changed(node, false);

  }

  void Solver::connection_conflicts(Connection &connection) {

  }

  void Solver::ripple_changed(Node &node) {
    for (auto &connection: node.get_connections()) {
      auto &neighbor = connection->get_other(node);
      if (!neighbor.is_resolved()) {
        node_changed(neighbor);
      }
    }
  }

  bool Solver::attempt_resolution(Node &node) {
    if (resolver.attempt_resolution(node, *this)) {
      resolve_node(node);
      ripple_changed(node);
      return true;
    }
    else {
      node_unchanged(node);
      return false;
    }
  }

  Solver::Progress Solver::process_unresolved() {
    Progress progress = Progress::none;
    for (int i = 0; i < unresolved.size(); ++i) {
      auto &node = *unresolved[i];
      if (node.is_resolved())
        continue;

      if (attempt_resolution(node))
        progress = Progress::some;
    }

    return progress;
  }

  Solver::Progress Solver::process_changed() {
    Progress progress = Progress::none;
    while (changed.size() > 0) {
      for (int i = 0; i < changed.size(); ++i) {
        auto &node = *changed[i];
        if (!node.is_changed())
          continue;

        if (attempt_resolution(node))
          progress = Progress::some;

        node_unchanged(node);
      }
    }
    return progress;
  }

  bool Solver::solve() {
    while (changed.size() != 0 || unresolved.size() != 0) {
      auto progress1 = process_unresolved();
      auto progress2 = process_changed();

      if (progress1 == Progress::none && progress2 == Progress::none)
        return false;
    }

    return true;
  }

}