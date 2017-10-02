#pragma once

#include "Solver.h"

namespace solving {
  Node *find_node(Graph &graph, source_mapping::Measurement row, source_mapping::Measurement column);
  void log_nodes(Graph &graph);
}