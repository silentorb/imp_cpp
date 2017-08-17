#pragma once

#include <overworld/schema/Dungeon.h>
#include "Variant.h"

namespace overworld {

  Node &clone_function(Function &function, Variant &variant);

  Node &clone_dungeon(Dungeon_Variant &variant);
}