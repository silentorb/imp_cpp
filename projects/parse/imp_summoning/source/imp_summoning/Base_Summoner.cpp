#include "Base_Summoner.h"
#include "exceptions.h"

using namespace std;

namespace imp_summoning {

  Base_Summoner::Base_Summoner(Stream &input, Lookup &lookup) :
    input(input),
    lookup(lookup),
    lexicon(runic_imp::Lexicon::get_instance().patterns){

  }

//  underworld::Member &Base_Summoner::find_member(const Token &rune, Context &context) {
//    auto name = rune.get_text();
//    auto member = context.find_member(name);
//    if (!member)
//      throw Syntax_Exception(input.current());
//
//    return *member;
//  }
}