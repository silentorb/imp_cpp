#pragma once

#include <overworld/schema/Dungeon.h>
#include <overworld/schema/Function_Variant.h>
#include "../Dungeon_Variant.h"

namespace overworld {

  Node &clone_function(Function_Variant &variant, Node &starting_node, Profession &new_profession, Graph &graph);

  Node &clone_dungeon(Dungeon_Variant &variant, Graph &graph);
}