#include "Function_Interface.h"
#include "Function.h"

namespace overworld {

  Function_Variant::Function_Variant(Function &original, Dungeon_Interface &dungeon,
                                     std::vector<Profession_Reference> &professions) :
    variant(original.get_name(),original.get_generic_parameters(), professions), original(original), dungeon(&dungeon) {}
}