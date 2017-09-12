#include "Base_Summoner.h"
#include "runic/common/exceptions.h"

using namespace std;
using namespace underworld;

namespace imp_summoning {

  Base_Summoner::Base_Summoner(runic::Stream &input, Parser_Lookup &lookup) :
    input(input),
    lookup(lookup),
    lexicon(lookup.get_lexicon()) {

  }

  underworld::Profession_Owner Base_Summoner::process_profession_token(Context &context) {
    auto name = input.current().get_text();
    if (input.peek().is(lexicon.dot)) {
      input.next();
      if (input.next().is_not(lexicon.identifier))
        throw runic::Syntax_Exception(input.current());

      auto child = process_profession_token(context);
      return Profession_Owner(new Dungeon_Reference_Profession(name, child, get_source_point()));
    }
    else {
      return Profession_Owner(new Token_Profession(name, get_source_point()));
    }
  }

  underworld::Profession_Owner Base_Summoner::process_profession(Context &context) {
    if (input.current().is(lexicon.ampersand)) {
      return Profession_Owner(new Reference(Profession_Owner(new Unknown(get_source_point()))));
    }
    auto primitive = lookup.get_primitive(input.current().get_match().get_type());
    if (primitive != Primitive_Type::Unknown)
      return Profession_Owner(new Primitive(primitive, get_source_point()));

    return process_profession_token(context);
  }

}