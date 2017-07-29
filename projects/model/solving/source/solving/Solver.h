#pragma once

#include "overworld/imp_graph/Node.h"
#include "overworld/imp_graph/Connection.h"
#include <vector>
#include <graphing/Reference_Graph.h>

namespace solving {

  using Node = overworld::Node;
  using Connection = overworld::Connection;

  class Solver {
      graphing::Reference_Graph<Node, Connection> &graph;

      std::vector<Node *> unresolved;
      std::vector<Node *> changed;

      std::vector<Connection *> conflicts;

//      Solver_Logic &resolver;

      enum Progress {
          none,
          some
      };

      bool attempt_resolution(Node &node);
      Progress process_unresolved();
      Progress process_changed();
      void set_node_resolved(Node &node);
      void node_unchanged(Node &node);
      void node_changed(Node &node);
      bool process_node(Node &node);

      int update_unresolved();

  public:
      Solver(graphing::Reference_Graph<Node, Connection> &graph) :
        graph(graph) {}

      bool solve();
      void ripple_changed(Node &node);
      void connection_conflicts(Connection &connection);

      // Assumes that this is the first scan and there is not existing scan result data.
      void scan_fresh();
      void add_node(Node &node);

      std::vector<Node *> &get_unsolved_nodes() {
        return unresolved;
      }
  };
}