#include <overworld/schema/professions/Profession.h>
#include "Ownership_Solver.h"
#include "../Generic_Solver.h"
#include "../solving_types.h"

using namespace overworld;
using namespace solving;

namespace lifetime {

//  Ownership get_transfer_ownership(const Node &node) {
//    switch (node.get_ownership()) {
//      case Lifetime_Ownership::owner:
//      case Lifetime_Ownership::value:
//      case Lifetime_Ownership::pointer:
//        return Ownership::reference;
//
//      default:
//        throw std::runtime_error("Not supported");
//    }
//  }

  class Ownership_Interface {
      Graph &graph;
      solving::Generic_Solver<Ownership_Interface, Node> generic_solver;

  public:
      Ownership_Interface(Graph &graph) :
        graph(graph),
        generic_solver(*this) {}

      Nodes &get_nodes() {
        return graph.get_nodes();
      }

      static bool is_resolved(const Node &node) {
        return node.get_ownership() != Lifetime_Ownership::unknown;
      }

      Lifetime_Ownership get_forward_ownership(Lifetime_Ownership ownership) {
        switch (ownership) {

          case Lifetime_Ownership::anchor:
            return Lifetime_Ownership::reference;

          case Lifetime_Ownership::copy:
            return Lifetime_Ownership::copy;

          case Lifetime_Ownership::move:
            return Lifetime_Ownership::anchor;

          case Lifetime_Ownership::reference:
            return Lifetime_Ownership::reference;

          default:
            throw std::runtime_error("Not supported");
        }
      }

      Lifetime_Ownership get_backwards_ownership(Lifetime_Ownership ownership) {
        switch (ownership) {

          case Lifetime_Ownership::anchor:
            return Lifetime_Ownership::move;

          case Lifetime_Ownership::copy:
            return Lifetime_Ownership::copy;

          case Lifetime_Ownership::move:
            return Lifetime_Ownership::move;

          case Lifetime_Ownership::reference:
            return Lifetime_Ownership::unknown;

          default:
            throw std::runtime_error("Not supported");
        }
      }


      Progress try_push(Node &first, Node &second, Connection &connection, Direction not_used) {
        if (second.get_ownership() != Lifetime_Ownership::unknown)
          throw std::runtime_error("Should not be setting ownership of a known node.");

        auto is_forward = &connection.get_first() == &first;
        auto ownership = first.get_ownership();

        auto new_ownership = is_forward
                             ? get_forward_ownership(ownership)
                             : get_backwards_ownership(ownership);

        if (new_ownership == Lifetime_Ownership::unknown)
          return 0;

        second.set_ownership(new_ownership);
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

  Ownership try_ownership(Node &node) {
//    auto &profession = node.get_profession();
//    if (node.get_element().get_type() == Element_Type::instantiation) {
//      if (profession.get_type() == Profession_Type::dungeon) {
//        auto &dungeon = profession->get_dungeon_interface().get_original();
//        if (dungeon.has_enchantment(Enchantment_Library::get_value())) {
//          return overworld::Ownership::value;
//        }
//      }
//      return Ownership::owner;
//    }

    return Ownership::unknown;
  }

  void determine_types(Graph &graph) {
//    for (auto &node: graph.get_nodes()) {
//      if (node->get_profession().get_ownership() == Ownership::unknown) {
//        auto ownership = try_ownership(*node);
//        if (ownership != Ownership::unknown)
//          node->get_profession().set_ownership(ownership);
//      }
//    }
  }

  void Ownership_Solver::post_apply() {

  }

  void Ownership_Solver::solve() {
    determine_types(graph);
    interface->solve();
    post_apply();
  }
}