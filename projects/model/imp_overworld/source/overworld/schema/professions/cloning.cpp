#include <overworld/schema/Dungeon.h>
#include <set>
#include <overworld/schema/Function.h>
#include "cloning.h"

namespace overworld {

  struct Dungeon_Functions {
      static Dungeon *get_context(Node &node) {
        return &node.get_element().get_parent().get_dungeon();
      }

      static Node_Copy *create_node(Node &other, Dungeon &variant) {
        return new Node_Copy(other, other.get_profession(), &variant, nullptr);
      }
  };

  struct Function_Functions {
      static Function *get_context(Node &node) {
        return &node.get_element().get_parent().get_function();
      }

      static Node_Copy *create_node(Node &other, Function &variant) {
        throw std::runtime_error("Not implemented.");
//        return new Node_Copy(other, other.get_profession(), &variant..get_dungeon(), &variant);
      }
  };

  template<typename Functions, typename V>
  void expand(Node_Copy &node, V &variant, Graph &graph, std::set<Node *> original_nodes) {
    original_nodes.insert(&node.get_original());

    for (auto connection : node.get_original().get_connections()) {
      auto &other = connection->get_other(node);
      if (Functions::get_context(other) == &variant.get_original() && !original_nodes.count(&other)) {
        auto new_node = Functions::create_node(other, variant);
        variant.add_node(Node_Owner(new_node));
        if (&node == &connection->get_first()) {
          graph.connect(node, *new_node);
        }
        else {
          graph.connect(*new_node, node);
        }
        expand<Functions>(*new_node, variant, graph, original_nodes);
      }
    }
  }

  Node &clone_function_graph(Function &variant, Node &starting_node, Profession_Reference &new_profession,
                             Graph &graph) {
    throw std::runtime_error("No longer handled this way.");
//    auto &function = variant.get_original();
//    auto node = new Node_Copy(starting_node, new_profession, starting_node.get_dungeon(), &variant);
//    variant.add_node(Node_Owner(node));
//    std::set<Node *> nodes;
//    expand<Function_Functions>(*node, variant, graph, nodes);
//    return *node;
  }

  Node &clone_dungeon_graph(Dungeon &variant, Graph &graph) {
    throw std::runtime_error("Not implemented.");

//    auto node = new Node_Copy(dungeon.get_node(), variant, &variant, nullptr);
//    variant.add_node(Node_Owner(node));
//    std::set<Node *> nodes;
//    expand<Dungeon_Functions>(*node, variant, graph, nodes);
//    return *node;
  }

}