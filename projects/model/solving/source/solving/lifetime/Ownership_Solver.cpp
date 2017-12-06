#include <overworld/schema/professions/Profession.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon_Variant.h>
#include <overworld/schema/professions/Enchantment_Library.h>
#include "Ownership_Solver.h"
#include "../Generic_Solver.h"
#include "../solving_types.h"

using namespace overworld;
using namespace solving;

namespace lifetime {

  class Ownership_Interface {
      Graph &graph;
      solving::Generic_Solver<Ownership_Interface, Node> generic_solver;

  public:
      Ownership_Interface(Graph &graph) :
        graph(graph),
        generic_solver(*this) {}

      std::vector<Node *> &get_nodes() {
        return graph.get_nodes();
      }

      static bool is_resolved(const Node &node) {
        return node.get_ownership() != overworld::Ownership::unknown;
      }

      overworld::Ownership get_forward_ownership(overworld::Ownership ownership) {
        switch (ownership) {

          case overworld::Ownership::owner:
            return overworld::Ownership::reference;

          case overworld::Ownership::copy:
            return overworld::Ownership::copy;

          case overworld::Ownership::move:
          case overworld::Ownership::implicit_move:
            return overworld::Ownership::owner;

          case overworld::Ownership::reference:
            return overworld::Ownership::reference;

          default:
            throw std::runtime_error("Not supported");
        }
      }

      overworld::Ownership get_backwards_ownership(overworld::Ownership ownership) {
        switch (ownership) {

          case overworld::Ownership::owner:
            return overworld::Ownership::move;

          case overworld::Ownership::copy:
            return overworld::Ownership::copy;

          case overworld::Ownership::move:
          case overworld::Ownership::implicit_move:
            return overworld::Ownership::move;

          case overworld::Ownership::reference:
            return overworld::Ownership::reference;

          default:
            throw std::runtime_error("Not supported");
        }
      }

      Progress try_push(Node &first, Node &second, Connection &connection, Direction not_used) {
        if (second.get_ownership() != overworld::Ownership::unknown)
          return 0;

//          throw std::runtime_error("Should not be setting ownership of a known node.");

        auto is_forward = &connection.get_first() == &first;
        auto ownership = first.get_ownership();

        auto new_ownership = is_forward
                             ? get_forward_ownership(ownership)
                             : get_backwards_ownership(ownership);

        if (new_ownership == overworld::Ownership::unknown)
          return 0;

        second.set_ownership(new_ownership);
        auto &first_profession = first.get_profession();
        auto first_variant = first_profession->as_variant();
        auto second_variant = second.get_profession()->as_variant();
        if (first_variant && second_variant && first_variant->is_generic() && second_variant->is_generic()) {
          auto &a2 = first_variant->get_arguments()[0]->get_node();
          auto &b2 = second_variant->get_arguments()[0]->get_node();
//          b.set_ownership(a.get_ownership());
          auto &a = first_variant->get_arguments()[0]->get_node();
          auto &b = second_variant->get_arguments()[0]->get_node();
          b.set_ownership(a.get_ownership());
        }
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

  void Ownership_Solver::solve() {
    determine_types(graph);
    interface->solve();
//    post_apply();
  }
}