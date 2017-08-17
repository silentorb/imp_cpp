#include <overworld/schema/Dungeon_Variant.h>
#include "cloning.h"

namespace overworld {

//  Node &clone_function(Function &function, Variant &variant) {
//
//  }

  Node &clone_dungeon(Dungeon_Variant &variant, Graph &graph) {
    auto &dungeon = variant.get_original();
    auto &node = variant.add_node(Node_Owner(new Node_Copy(dungeon.get_node(), variant, variant)));
    return node;
  }
}