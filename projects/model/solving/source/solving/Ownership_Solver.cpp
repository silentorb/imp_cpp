#include "Ownership_Solver.h"
#include "Generic_Solver.h"
#include "Solving_Visualizer.h"

using namespace overworld;

namespace solving {

  Ownership get_transfer_ownership(const Node &node) {
    auto &profession = node.get_profession();
    auto ownership = profession.get_ownership();
    switch (ownership) {
      case Ownership::owner:
      case Ownership::value:
      case Ownership::pointer:
        return Ownership::reference;

      default:
        throw std::runtime_error("Not supported");
    }
  }

  class Ownership_Interface {
      Graph &graph;
      Generic_Solver<Ownership_Interface, Node> generic_solver;

  public:
      Ownership_Interface(Graph &graph) :
        graph(graph),
        generic_solver(*this) {}

      std::vector<Node *> &get_nodes() {
        return graph.get_nodes();
      }

      static bool is_resolved(const Node &node) {
        return node.get_profession().get_ownership() != Ownership::unknown;
      }

      Progress try_push(Node &first, Node &second, Connection &connection, Direction direction) {
        std::cout << get_node_ownership_string(first) << " > " << get_node_ownership_string(second) << std::endl;
        auto ownership = get_transfer_ownership(first);
        second.get_profession().set_ownership(ownership);
        return 1;
      }

      void on_changed(Node &node) {

      }

      void solve() {
        generic_solver.solve();
      }
  };

  Ownership_Solver::Ownership_Solver(Graph &graph) :
    graph(graph),
    interface(new Ownership_Interface(graph)) {

  }

  Ownership_Solver::~Ownership_Solver() {}

//  void Ownership_Solver::update_node(Node &node) {
//    auto &profession = node.get_profession();
//    auto ownership = profession.get_ownership();
//    if (ownership != Ownership::unknown)
//      return;
//
//    for (auto &neighbor : node.get_neighbors()) {
//      auto neighbor_ownership = neighbor->get_profession().get_ownership();
//      if (neighbor_ownership != Ownership::unknown) {
//
//      }
//    }
//  }

  void Ownership_Solver::solve() {
    interface->solve();
//    for (auto &node: graph.get_nodes()) {
//      update_node(*node);
//    }
  }


}