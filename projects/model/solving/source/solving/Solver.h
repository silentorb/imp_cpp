#pragma once

#include "overworld/imp_graph/Node.h"
#include "overworld/imp_graph/Connection.h"
#include <vector>
#include <graphing/Reference_Graph.h>
#include <overworld/schema/professions/Profession_Library.h>

namespace solving {

  using Node = overworld::Node;
  using Connection = overworld::Connection;

  using Progress = unsigned int;

  class Solver {
      graphing::Reference_Graph<Node, Connection> &graph;
      std::vector<Node *> unresolved;
      std::vector<Node *> changed_buffers[2];
      std::vector<Node *> *changed = &changed_buffers[0];
      std::vector<Node *> *next_changed = &changed_buffers[1];
      std::vector<Connection *> conflicts;
      overworld::Profession_Library &profession_library;

#ifdef DEBUG_SOLVER
      std::vector<Node *> resolved;
#endif

      void initialize();
      Progress process_changed();
      Progress process_conflicts();
      Progress ripple(Node &node);

      int update_unresolved();
      int update_changed();
      int update_conflicts();
      bool has_unresolved_nodes();

      void set_profession(Node &node, overworld::Profession &profession);
      void set_changed(Node &node);

      void on_add(Node &node);
      void on_remove(Node &node);
      void on_connect(Connection  & connection);
      Progress inhale(Node &node);

  public:
      Solver(graphing::Reference_Graph<Node, Connection> &graph, overworld::Profession_Library &profession_library) :
        graph(graph), profession_library(profession_library) {
        graph.set_on_add([this](Node &node) { on_add(node); });
        graph.set_on_remove([this](Node &node) { on_remove(node); });
        graph.set_on_connect([this](Connection &connection) { on_connect(connection); });
      }

      bool solve();
//      void ripple_changed(Node &node);
//      void connection_conflicts(Connection &connection);

      // Assumes that this is the first scan and there is not existing scan result data.
      void scan_fresh();
      void add_node(Node &node);

      std::vector<Node *> &get_unsolved_nodes() {
        update_unresolved();
        return unresolved;
      }

      void log_nodes();
  };
}