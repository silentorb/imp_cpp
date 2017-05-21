#pragma once

#include "Node.h"

namespace graphing {
  template <typename Node, typename Connection>
  class Reference_Graph;
}

namespace overworld {

  class Graph {
      std::unique_ptr<graphing::Reference_Graph<Node, solving::Connection>> graph;

  public:
      Graph();
      virtual ~Graph();
      void add_node(Node & node);
      solving::Connection &connect(Node &first, Node &second);
  };
}