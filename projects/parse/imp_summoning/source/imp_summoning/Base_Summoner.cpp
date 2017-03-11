#include "Base_Summoner.h"
#include "exceptions.h"

using namespace std;

namespace imp_summoning {

  Base_Summoner::Base_Summoner(Stream &input, Lookup &lookup) :
    input(input),
    lookup(lookup),
    lexicon(runic_imp::Lexicon::get_instance().patterns),
    profession_library(lookup.get_profession_library()) {

  }

}