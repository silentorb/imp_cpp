#pragma once

#include <graphing/Reference_Graph.h>
#include "Node.h"

//namespace graphing {
//  template<typename Node, typename Connection>
//  class Reference_Graph;
//}

namespace overworld {

//  class Graph {
//      std::unique_ptr<graphing::Reference_Graph<Node, Connection>> graph;
//
//      void add_node(Node &node);
//      bool has_node(Node &node);
//
//  public:
//      Graph();
//      virtual ~Graph();
//      Connection &connect(Node &first, Node &second);
//
//      graphing::Reference_Graph<Node, Connection> &get_graph() {
//        return *graph;
//      }
//  };

  class Graph : public graphing::Reference_Graph<Node, Connection> {

  public:
//      Connection &connect(Node &first, Node &second) {
//        return connect(first, second,
//                       std::unique_ptr<Connection>(new Connection(first, second))
//        );
//      }
  };

//  using Graph = graphing::Reference_Graph<Node, Connection>;
}