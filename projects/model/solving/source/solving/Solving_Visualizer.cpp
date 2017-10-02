#include "Solving_Visualizer.h"
#include <iostream>

using namespace overworld;

namespace solving {

  unsigned int get_row(overworld::Node &node) {
    return node.get_profession_reference().get_source_point().get_row();
  }

  bool source_point_is_before(const source_mapping::Source_Range &first, const source_mapping::Source_Range &second) {
    if (first.get_row() < second.get_column()) {
      if (first.get_row() == second.get_row()) {
        return first.get_column() < second.get_column();
      }
      return true;
    }
    return false;
  }

  void insert_node(std::vector<overworld::Node *> &nodes, overworld::Node *node) {
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
      auto first = node->get_profession_reference().get_source_point();
      auto second = (*i)->get_profession_reference().get_source_point();
      if (source_point_is_before(first, second)) {
        nodes.insert(i, node);
        return;
      }
    }

    nodes.push_back(node);
  }

  void log_node(overworld::Node &node) {
    std::cout << node.get_debug_string();
    std::cout << std::endl;
  }

  Node *find_node(Graph &graph, source_mapping::Measurement row, source_mapping::Measurement column) {
    for (auto node: graph.get_nodes()) {
      auto source_point = node->get_profession_reference().get_source_point();
      if (source_point.get_row() == row && source_point.get_column() == column)
        return node;
    }
    return nullptr;
  }

  void log_nodes(graphing::Reference_Graph<Node, Connection> &graph) {
    std::cout << std::endl << "Logging nodes:" << std::endl;

    std::vector<overworld::Node *> nodes;
    for (auto first : graph.get_nodes()) {
      insert_node(nodes, first);
    }
    for (auto &node : nodes) {
      log_node(*node);
      for (auto &other : node->get_neighbors()) {

        std::cout << " * ";

        log_node(*other);
      }
    }

    std::cout << std::endl;
  }
}