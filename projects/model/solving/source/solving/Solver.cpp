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
    for (auto conflict: conflict_manager.conflicts) {
      if (&conflict.get_connection() == &connection)
        return;
    }

    conflict_manager.conflicts.push_back({connection, false});
  }

  bool Solver::assignment_is_compatible(overworld::Profession &first, overworld::Profession &second) {
    return can_cast(first, second);
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

  void get_ancestors_recursive(Profession &profession, std::vector<Profession *> &buffer) {
    auto contracts_or_null = profession.get_contracts();
    if (!contracts_or_null)
      return;

    auto &contracts = *contracts_or_null;

    for (auto contract : contracts) {
      buffer.push_back(contract);
    }

    for (auto contract : contracts) {
      get_ancestors_recursive(*contract, buffer);
    }
  }

  void get_ancestors(Profession &profession, std::vector<Profession *> &buffer) {
    buffer.clear();
    get_ancestors_recursive(profession, buffer);
  }

  bool buffer_has_item(std::vector<Profession *> &buffer, Profession *item) {
    for (auto entry: buffer) {
      if (entry == item)
        return true;
    }

    return false;
  }

  Profession *find_first_match(std::vector<Profession *> &first_buffer, std::vector<Profession *> &second_buffer) {
    for (auto first_step : first_buffer) {
      if (buffer_has_item(second_buffer, first_step))
        return first_step;
    }

    return nullptr;
  }

  bool Solver::resolve_conflict(Connection &connection) {
    auto &first = connection.get_first();
    auto &second = connection.get_second();
    auto &first_profession = first.get_profession();
    auto &second_profession = second.get_profession();

    get_ancestors(first_profession, ancestors_buffer1);
    get_ancestors(second_profession, ancestors_buffer2);

    auto common_ancestor = find_first_match(ancestors_buffer1, ancestors_buffer2);
    if (common_ancestor) {
      set_profession(first, *common_ancestor);
      return true;
    }

    return false;
  }

  Progress Solver::process_conflicts() {
    Progress progress = 0;
    auto i = conflict_manager.conflicts.begin();
    while (i != conflict_manager.conflicts.end()) {
      auto &conflict = *i;
      if (!conflict.attempted_resolution) {
        if (resolve_conflict(conflict.get_connection())) {
          ++progress;
          conflict_manager.conflicts.erase(i++);
          continue;
        }
        else {
          conflict.attempted_resolution = true;
        }
      }
      ++i;
    }
    return progress;
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
    return conflict_manager.conflicts.size();
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
        if (has_unresolved_nodes() || conflict_manager.conflicts.size() > 0)
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

  unsigned int get_row(overworld::Node &node) {
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
#if DEBUG_SOLVER
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
#endif
  }
}