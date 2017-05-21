#pragma once

#include "Node.h"
#include "Connection.h"
#include <vector>
#include <graphing/Reference_Graph.h>

namespace solving {

  class Solver;

  class Solver_Logic {
  public:
      virtual bool attempt_resolution(Node &node, Solver &graph) = 0;
  };

  class Solver {
      graphing::Reference_Graph<Node, Connection> &graph;

      std::vector<Node *> unresolved;
      std::vector<Node *> changed;

      std::vector<Connection *> conflicts;

      Solver_Logic &resolver;

      enum Progress {
          none,
          some
      };

      bool attempt_resolution(Node &node);
      Progress process_unresolved();
      Progress process_changed();
      void resolve_node(Node &node);
      void node_unchanged(Node &node);
      void node_changed(Node &node);

  public:
      Solver(graphing::Reference_Graph<Node, Connection> &graph, Solver_Logic &resolver) :
        graph(graph), resolver(resolver) {}

      bool solve();
      void add_node(Node *node);
      Connection &connect(Node &first, Node &second);
      void ripple_changed(Node &node);
      void connection_conflicts(Connection &connection);

  };
}