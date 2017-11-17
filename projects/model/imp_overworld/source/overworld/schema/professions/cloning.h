#pragma once

#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Function.h>
#include "../Dungeon.h"

namespace overworld {

  Node &clone_function_graph(Function &variant, Node &starting_node, Profession_Reference &new_profession, Graph &graph);

  Node &clone_dungeon_graph(Dungeon &variant, Graph &graph);
}