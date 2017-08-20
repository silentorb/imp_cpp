#pragma once

#include "overworld/imp_graph/Node.h"
#include "overworld/imp_graph/Connection.h"
#include <vector>
#include <list>
#include <graphing/Reference_Graph.h>
#include <overworld/schema/professions/Profession_Library.h>

namespace solving {

  using Node = overworld::Node;
  using Connection = overworld::Connection;

  using Progress = unsigned int;

  struct Conflict {
      Connection *connection;
      bool attempted_resolution;

      Conflict(Connection &connection, bool attempted_resolution = false) :
        connection(&connection), attempted_resolution(attempted_resolution) {}

      Connection &get_connection() {
        return *connection;
      }

      const Connection &get_connection() const {
        return *connection;
      }
  };

  // Working around a bug in CLion debugger not being able to render lists.
  struct Conflict_Manager {
      std::list<Conflict> conflicts;
  };

  class Solver {
      graphing::Reference_Graph<Node, Connection> &graph;
      std::vector<Node *> unresolved;
      std::vector<Node *> changed_buffers[2];
      std::vector<Node *> *changed = &changed_buffers[0];
      std::vector<Node *> *next_changed = &changed_buffers[1];
      overworld::Profession_Library &profession_library;
      Conflict_Manager conflict_manager;
      std::vector<overworld::Profession *> ancestors_buffer1;
      std::vector<overworld::Profession *> ancestors_buffer2;

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

      bool resolve_conflict(Connection &connection);
      void set_profession(Node &node, overworld::Profession &profession);
      void set_changed(Node &node);

      bool assignment_is_compatible(overworld::Profession &first, overworld::Profession &second);
      void check_node_conflicts(Node &node);
      bool is_conflict(Connection &connection);
      void on_add(Node &node);
      void on_remove(Node &node);
      void on_connect(Connection &connection);
      Progress inhale(Node &node);
      void add_conflict(Connection &connection);

      void resolve_with_template_function(Connection &connection);

      void create_function_variant(overworld::Function_Variant_Array &variant_array,
                                   overworld::Function &function,
                                   Node &starting_node, overworld::Profession &profession);

  public:
      Solver(graphing::Reference_Graph<Node, Connection> &graph, overworld::Profession_Library &profession_library) :
        graph(graph), profession_library(profession_library) {
        graph.set_on_add([this](Node &node) { on_add(node); });
        graph.set_on_remove([this](Node &node) { on_remove(node); });
        graph.set_on_connect([this](Connection &connection) { on_connect(connection); });
      }

      bool solve();

      // Assumes that this is the first scan and there is not existing scan result data.
      void scan_fresh();
      void add_node(Node &node);

      std::vector<Node *> &get_unsolved_nodes() {
        update_unresolved();
        return unresolved;
      }

      void log_nodes();

      const std::list<Conflict> &get_conflicts() const {
        return conflict_manager.conflicts;
      }
  };
}