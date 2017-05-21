#include "Graph.h"
#include "graphing/Reference_Graph.h"

namespace overworld {

  Graph::Graph() :
    graph(std::unique_ptr<graphing::Reference_Graph<Node, solving::Connection>>(
      new graphing::Reference_Graph<Node, solving::Connection>())) {}

  Graph::~Graph() {

  }

  void Graph::add_node(Node &node) {
    graph->add_node(node);
  }

  solving::Connection &Graph::connect(Node &first, Node &second) {
    return graph->connect(first, second);
  }

}