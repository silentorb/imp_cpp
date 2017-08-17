#pragma once

#include <overworld/schema/Dungeon.h>
#include "../Dungeon_Variant.h"

namespace overworld {

//  Node &clone_function(Function &function, Variant &variant);

  Node &clone_dungeon(overworld::Dungeon_Variant &variant, Graph &graph);
}