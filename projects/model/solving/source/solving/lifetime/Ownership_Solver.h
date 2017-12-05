#pragma once

namespace lifetime {

  template<typename Interface, typename Node>
  class Generic_Solver;

  class Ownership_Interface;

  class Ownership_Solver {
      overworld::Graph &graph;
//      std::unique_ptr<Generic_Solver<Ownership_Solver, Node>> generic_solver;
      std::unique_ptr<Ownership_Interface> interface;
//      void update_node(Node &node);
      void post_apply();

  public:
      Ownership_Solver(overworld::Graph &graph);
      virtual ~Ownership_Solver();

      void solve();
  };
}