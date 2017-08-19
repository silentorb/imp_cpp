#include "Function_Interface.h"
#include "Function.h"

namespace overworld {

  Function_Variant::Function_Variant(Function &original, Dungeon_Interface &dungeon,
                                     const std::vector<Profession *> &professions) :
    Variant(original.get_name(), professions), original(original), dungeon(&dungeon) {}
}