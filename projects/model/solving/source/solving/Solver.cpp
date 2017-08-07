#include "Solver.h"
#include <algorithm>

#if DEBUG_SOLVER

#include <iostream>

#endif

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

  void Solver::add_conflict(Connection &connection) {
    for (auto conflict: conflicts) {
      if (conflict == &connection)
        return;
    }

    conflicts.push_back(&connection);
  }

  bool Solver::assignment_is_compatible(overworld::Profession &first, overworld::Profession &second) {
    // Oversimplication for now
    return &first.get_base() == &second.get_base();
  }

  bool Solver::is_conflict(Connection &connection) {
    auto &first = connection.get_first();
    auto &second = connection.get_second();
    if (!first.is_resolved() || !second.is_resolved())
      return false;

    if (!assignment_is_compatible(first.get_profession(), second.get_profession())) {
      return true;
    }

    return false;
  }

  void Solver::check_node_conflicts(Node &node) {
    for (auto connection: node.get_connections()) {
      if (is_conflict(*connection))
        add_conflict(*connection);
    }
  }

  void Solver::set_changed(Node &node) {
    node.set_changed(true);
    next_changed->push_back(&node);
    check_node_conflicts(node);
  }

  void Solver::set_profession(Node &node, overworld::Profession &profession) {
#if DEBUG_SOLVER
    if (profession.get_base().get_type() == overworld::Profession_Type::unknown)
      throw std::runtime_error("Invalid type.");
#endif

    auto &previous = node.get_profession_reference().get_profession();
    if (previous.get_type() == Profession_Type::reference) {
      node.set_profession(profession_library.get_reference(profession.get_base()));
    }
    else {
      node.set_profession(profession);
    }

    set_changed(node);
  }

  Progress Solver::inhale(Node &node) {
    for (auto other : node.get_neighbors()) {
      if (other->is_resolved()) {
#if DEBUG_SOLVER
        std::cout << "# " << node.get_debug_string() << " < " << other->get_debug_string() << std::endl;
#endif
        auto &profession = other->get_profession_reference().get_profession();
        set_profession(node, profession);
        return 1;
      }
    }

    return 0;
  }

  Progress Solver::ripple(Node &node) {
    Progress progress = 0;
    for (auto other : node.get_neighbors()) {
      if (!other->is_resolved()) {
#if DEBUG_SOLVER
        std::cout << "# " << node.get_debug_string() << " > " << other->get_debug_string() << std::endl;
#endif
        auto &profession = node.get_profession_reference().get_profession();
        set_profession(*other, profession);
        ++progress;
      }
    }

    return progress;
  }

  Progress Solver::process_changed() {
    Progress progress = 0;
    for (auto node : *changed) {
      if (!node)
        continue;

      if (node->is_resolved())
        progress += ripple(*node);
      else
        progress += inhale(*node);
    }
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
    changed->clear();

    // Flip buffers
    auto temp = changed;
    changed = next_changed;
    next_changed = temp;
    return changed->size();
  }

  bool Solver::has_unresolved_nodes() {
    for (auto node : graph.get_nodes()) {
      if (!node->is_resolved())
        return true;
    }

    return false;
  }

  int Solver::update_conflicts() {
    return conflicts.size();
  }

  void Solver::on_add(Node &node) {
    set_changed(node);
  }

  void Solver::on_connect(Connection &connection) {
    set_changed(connection.get_first());
    set_changed(connection.get_second());
  }

  void Solver::on_remove(Node &node) {
    for (auto i = 0; i < changed->size(); ++i) {
      if ((*changed)[i] == &node) {
        (*changed)[i] = nullptr;
        break;
      }
    }
  }

  void Solver::initialize() {
    for (auto node : graph.get_nodes()) {
      if (node->is_resolved())
        set_changed(*node);
    }

#if DEBUG_SOLVER
    std::cout << "Changed Nodes:" << std::endl;

    for (auto node : *next_changed) {
      std::cout << "  " << node->get_debug_string() << std::endl;
    }
#endif
  }

  bool Solver::solve() {
    initialize();

    while (update_changed() || update_conflicts()) {
      auto progress = process_changed() +
                      process_conflicts();

      if (progress == 0) {
        if (has_unresolved_nodes() || conflicts.size() > 0)
          return false;
      }
    }

    return true;
  }

  void log_node(overworld::Node &node) {
#if DEBUG_SOLVER
    std::cout << node.get_debug_string();
    std::cout << std::endl;
#endif
  }

  int get_row(overworld::Node &node) {
    return node.get_profession_reference().get_source_point().get_row();
  }

  void insert_node(std::vector<overworld::Node *> &nodes, overworld::Node *node) {
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      if (get_row(*node) < get_row(**i)) {
        nodes.insert(i, node);
        return;
      }
    }

    nodes.push_back(node);
  }

  void Solver::log_nodes() {
    std::vector<overworld::Node *> nodes;
    for (auto first: graph.get_nodes()) {
      insert_node(nodes, first);
    }
    for (auto &node : nodes) {
      log_node(*node);
      for (auto &other : node->get_neighbors()) {
        std::cout << " * ";
        log_node(*other);
      }
    }
  }
}