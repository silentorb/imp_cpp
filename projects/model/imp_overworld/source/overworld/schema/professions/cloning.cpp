#include <overworld/schema/Dungeon_Variant.h>
#include <set>
#include "cloning.h"

namespace overworld {

//  Node &clone_function(Function &function, Variant &variant) {
//
//  }

  void expand(Node_Copy &node, Dungeon_Variant &variant, Graph &graph, std::set<Node *> nodes) {
    nodes.insert(&node);

    for (auto connection :node.get_original().get_connections()) {
      auto &other = connection->get_other(node);
      if (&other.get_dungeon() == &variant.get_original() && !nodes.count(&other)) {
        auto new_node = new Node_Copy(other, variant, variant, nullptr);
        variant.add_node(Node_Owner(new_node));
        if (&node == &connection->get_first()) {
          graph.connect(node, *new_node);
        }
        else {
          graph.connect(*new_node, node);
        }

        expand(*new_node, variant, graph, nodes);
      }
    }
  }

  Node &clone_dungeon(Dungeon_Variant &variant, Graph &graph) {
    auto &dungeon = variant.get_original();
    auto node = new Node_Copy(dungeon.get_node(), variant, variant, nullptr);
    variant.add_node(Node_Owner(node));
    std::set<Node *> nodes;
    expand(*node, variant, graph, nodes);
    return *node;
  }
}