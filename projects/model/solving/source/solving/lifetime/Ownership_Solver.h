#pragma once

#include "lifetime/Profession_Solver.h"

namespace lifetime {

  template<typename Interface, typename Node>
  class Generic_Solver;

  class Ownership_Interface;

  class Ownership_Solver {
      Graph &graph;
//      std::unique_ptr<Generic_Solver<Ownership_Solver, Node>> generic_solver;
      std::unique_ptr<Ownership_Interface> interface;
//      void update_node(Node &node);

  public:
      Ownership_Solver(Graph &graph);
      virtual ~Ownership_Solver();

      void solve();
  };
}