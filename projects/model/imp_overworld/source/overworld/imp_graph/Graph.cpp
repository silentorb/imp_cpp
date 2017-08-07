#include "Graph.h"
#include "graphing/Reference_Graph.h"

namespace overworld {

//  Graph::Graph() :
//    graph(std::unique_ptr<graphing::Reference_Graph<Node, Connection>>(
//      new graphing::Reference_Graph<Node, Connection>())) {}
//
//  Graph::~Graph() {
//
//  }
//
//  void Graph::add_node(Node &node) {
//    graph->add_node(node);
//  }
//
//  bool Graph::has_node(Node &node) {
//    for (auto &pointer : graph->get_nodes()) {
//      if (pointer == &node) {
//        return true;
//      }
//    }
//    return false;
//  }
//
//  Connection &Graph::connect(Node &first, Node &second) {
////    if (((int) first.is_changed()) > 1 || ((int) first.is_changed()) < 0)
////      throw std::runtime_error("Error with node");
////
////    if (((int) second.is_changed()) > 1 || ((int) second.is_changed()) < 0)
////      throw std::runtime_error("Error with node");
//
////    if (!has_node(first))
////        add_node(first);
////
////    if (!has_node(second))
////      add_node(second);
//
//    return graph->connect(first, second);
//  }

}