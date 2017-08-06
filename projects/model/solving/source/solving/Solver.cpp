#include "Solver.h"
#include <algorithm>

using namespace overworld;

namespace solving {

  void Solver::add_node(Node &node) {
    if (!node.is_resolved())
      unresolved.push_back(&node);
  }

  void Solver::scan_fresh() {
    for (auto node : graph.get_nodes()) {
      add_node(*node);
    }
  }

  void Solver::set_node_resolved(Node &node) {
    if (node.is_resolved())
      return;

    if (node.get_profession_reference().get_profession().get_base().get_type() == overworld::Profession_Type::unknown)
      throw std::runtime_error("Invalid resolution.");

    node.set_resolved(true);
  }

  void Solver::node_changed(Node &node) {
    if (node.is_changed())
      return;

//    changed.push_back(&node);
    node.set_changed(true);
  }

  void Solver::node_unchanged(Node &node) {
    if (!node.is_changed())
      return;

    changed.erase(std::remove(changed.begin(), changed.end(), &node), changed.end());
    node.set_changed(false);

  }

//  void Solver::connection_conflicts(Connection &connection) {
//
//  }

  void Solver::ripple_changed(Node &node) {
    for (auto &connection: node.get_connections()) {
      auto &neighbor = connection->get_other(node);
      if (!neighbor.is_resolved()) {
        node_changed(neighbor);
      }
    }
  }

  void Solver::set_profession(Node &node, overworld::Profession &profession) {
    if (node.get_profession_reference().get_source_point().get_row() == 12)
      int k = 0;
    if (profession.get_base().get_type() == overworld::Profession_Type::unknown)
      throw std::runtime_error("Invalid type.");
    auto &previous = node.get_profession_reference().get_profession();
    if (previous.get_type() == Profession_Type::reference) {
      node.set_profession(profession_library.get_reference(profession.get_base()));
    }
    else {
      node.set_profession(profession);
    }
  }

  bool Solver::process_node(Node &node) {
    for (auto other : node.get_neighbors()) {
      if (other->is_resolved()) {
        auto &profession = other->get_profession_reference().get_profession();
        set_profession(node, profession);
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

//  Progress Solver::process_unresolved() {
//    Progress progress = 0;
//    for (int i = 0; i < unresolved.size(); ++i) {
//      auto &node = *unresolved[i];
//      if (node.is_resolved())
//        continue;
//
//      if (attempt_resolution(node))
//        ++progress;
//    }
//
//    return progress;
//  }

  Progress Solver::process_changed() {
    Progress progress = 0;
//    while (changed.size() > 0) {
    for (int i = 0; i < changed.size(); ++i) {
      auto &node = *changed[i];
      if (!node.is_changed())
        continue;

      if (attempt_resolution(node))
        ++progress;

      node_unchanged(node);
    }
//    }
    return progress;
  }

  Progress Solver::process_conflicts() {
    return 0;
  }

  int Solver::update_unresolved() {
    unresolved.clear();
    for (auto node : graph.get_nodes()) {
      if (!node->is_resolved())
        unresolved.push_back(node);
    }

    return unresolved.size();
  }

  int Solver::update_changed() {
    changed.clear();
    for (auto node : graph.get_nodes()) {
      if (node->is_changed())
        changed.push_back(node);
    }

    return changed.size();
  }

  bool Solver::double_check_unresolved() {
    for (auto node : unresolved) {
      if (node->is_resolved())
        return false;
    }

    return true;
  }

  int Solver::update_conflicts() {
    return 0;
  }

  void Solver::initialize() {
    for (auto node : graph.get_nodes()) {
      if (!node->is_resolved())
        node->set_changed(true);
    }
  }

  bool Solver::solve() {
    initialize();

    while (update_changed() || update_conflicts()) {
//      for (auto node : graph.get_nodes()) {
//        node->set_changed(false);
//      }

      auto progress = process_changed()
                      + process_conflicts();

      if (progress == 0) {
        if (double_check_unresolved())
          return false;
      }
    }

    return true;
  }

}