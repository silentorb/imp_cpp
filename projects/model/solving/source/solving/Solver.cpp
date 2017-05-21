#include "Solver.h"
#include <algorithm>

namespace solving {

  void Solver::scan_fresh() {
    for (auto node : graph.get_nodes()) {
      if (!node->is_resolved())
        unresolved.push_back(node);
    }
  }

  void Solver::set_node_resolved(Node &node) {
    if (node.is_resolved())
      return;

    unresolved.erase(std::remove(unresolved.begin(), unresolved.end(), &node), unresolved.end());
    node.set_resolved(true);
  }

  void Solver::node_changed(Node &node) {
    if (node.is_changed())
      return;

    changed.push_back(&node);
    node.set_changed(true);
  }

  void Solver::node_unchanged(Node &node) {
    if (!node.is_changed())
      return;

    changed.erase(std::remove(changed.begin(), changed.end(), &node), changed.end());
    node.set_changed(false);

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

  bool Solver::process_node(Node &node) {
    for(auto other : node.get_neighbors()) {
      if(other->is_resolved()){
        auto & profession = other->get_profession_reference().get_profession();
        node.get_profession_reference().set_profession(profession);
        return true;
      }
    }

    return false;
  }

  bool Solver::attempt_resolution(Node &node) {
    if (process_node(node)) {
      set_node_resolved(node);
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