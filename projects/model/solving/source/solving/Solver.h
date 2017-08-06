#pragma once

#include "overworld/imp_graph/Node.h"
#include "overworld/imp_graph/Connection.h"
#include <vector>
#include <graphing/Reference_Graph.h>
#include <overworld/schema/professions/Profession_Library.h>

namespace solving {

  using Node = overworld::Node;
  using Connection = overworld::Connection;

  class Solver {
      graphing::Reference_Graph<Node, Connection> &graph;
      std::vector<Node *> unresolved;
      std::vector<Node *> changed;
      std::vector<Connection *> conflicts;
      overworld::Profession_Library &profession_library;

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
      int update_changed();
      bool double_check_unresolved();

      void set_profession(Node &node, overworld::Profession &profession);

  public:
      Solver(graphing::Reference_Graph<Node, Connection> &graph, overworld::Profession_Library &profession_library) :
        graph(graph), profession_library(profession_library) {}

      bool solve();
      void ripple_changed(Node &node);
//      void connection_conflicts(Connection &connection);

      // Assumes that this is the first scan and there is not existing scan result data.
      void scan_fresh();
      void add_node(Node &node);

      std::vector<Node *> &get_unsolved_nodes() {
        return unresolved;
      }
  };
}