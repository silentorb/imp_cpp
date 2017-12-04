#pragma once

#include "Profession_Solver.h"

namespace solving {
  const std::string render_node_status(overworld::Node_Status status);
  const std::string render_ownership(const overworld::Profession_Reference &profession);
  const std::string get_node_debug_string(const Node &node);
  const std::string get_node_ownership_string(const Node &node);
  Node *find_node(Graph &graph, source_mapping::Measurement row, source_mapping::Measurement column);
  std::vector<Node *> find_nodes(Graph &graph, source_mapping::Measurement row, source_mapping::Measurement column);

  using Node_Info = std::function<const std::string(const overworld::Node &node)>;
  void log_nodes(Graph &graph, Node_Info node_info = get_node_debug_string);
  void log_node_trees(overworld::Graph &graph);

}