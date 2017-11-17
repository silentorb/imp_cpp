#include "Basic_Dungeon.h"
#include "Dungeon_Variant.h"

namespace overworld {

  Dungeon_Variant *Basic_Dungeon::as_variant() {
    return dynamic_cast<Dungeon_Variant*>(this);
  }
}