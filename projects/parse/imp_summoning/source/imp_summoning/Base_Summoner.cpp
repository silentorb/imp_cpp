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
    auto name = input.expect(lexicon.identifier).get_text();
    if (input.next().is(lexicon.dot)) {
      input.expect_next(lexicon.identifier);
      auto child = process_profession_token(context);
      return Profession_Owner(new Dungeon_Reference_Profession(name, child, get_source_point()));
    }
    else {
      return Profession_Owner(new Token_Profession(name, get_source_point()));
    }
  }

  underworld::Profession_Owner Base_Summoner::process_profession_internal(Context &context) {
    if (input.current().is(lexicon.ampersand)) {
      input.next();
      auto child = process_profession(context);
      return Profession_Owner(new Reference(std::move(child)));
    }

    if (input.current().is(lexicon.question_mark)) {
      input.next();
      return Profession_Owner(new Unknown(get_source_point()));
    }

    auto primitive = lookup.get_primitive(input.current().get_match().get_type());
    if (primitive != Primitive_Type::Unknown) {
      input.next();
      return Profession_Owner(new Primitive(primitive, get_source_point()));
    }

    return process_profession_token(context);
  }

  underworld::Profession_Owner Base_Summoner::process_profession(Context &context) {
    auto profession = process_profession_internal(context);
    if (input.current().is(lexicon.single_arrow)) {
      input.next();
      auto child = process_profession(context);

    }
    return profession;
  }

}