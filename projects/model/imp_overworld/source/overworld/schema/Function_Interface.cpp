#include "Function_Interface.h"
#include "Function.h"

namespace overworld {

  Function_Variant::Function_Variant(Virtual_Function &original, Dungeon_Interface &dungeon,
                                     const std::vector<Profession *> &professions) :
    Variant(original.get_name(),original.get_generic_parameters(), professions), original(original), dungeon(&dungeon) {}
}