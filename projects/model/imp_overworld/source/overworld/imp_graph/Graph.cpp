#include "Graph.h"
#include "graphing/Reference_Graph.h"

namespace overworld {

  Graph::Graph() :
    graph(std::unique_ptr<graphing::Reference_Graph<Node, Connection>>(
      new graphing::Reference_Graph<Node, Connection>())) {}

  Graph::~Graph() {

  }

  void Graph::add_node(Node &node) {
    graph->add_node(node);
  }

  Connection &Graph::connect(Node &first, Node &second) {
    return graph->connect(first, second);
  }

}