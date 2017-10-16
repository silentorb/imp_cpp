#pragma once

#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Function_Variant.h>
#include "../Dungeon_Variant.h"

namespace overworld {

  Node &clone_function_graph(Function_Variant &variant, Node &starting_node, Profession_Reference &new_profession, Graph &graph);

  Node &clone_dungeon_graph(Dungeon_Variant &variant, Graph &graph);
}