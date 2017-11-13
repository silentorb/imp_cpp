#include <underworld/schema/Dungeon_Variant.h>
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

  void Base_Summoner::process_variant_arguments(underworld::Dungeon_Variant &variant, Context &context) {
    if (input.current().is(lexicon.greater_than))
      return;

    while (true) {
      variant.add_argument(process_profession(context));

      if (input.current().is_not(lexicon.comma))
        break;

      input.next();
    }

    input.expect(lexicon.greater_than);
    input.next();
  }

  underworld::Profession_Owner Base_Summoner::process_profession_token(Context &context) {
    auto name = input.expect(lexicon.identifier).get_text();
    if (input.next().is(lexicon.dot)) {
      input.expect_next(lexicon.identifier);
      auto child = process_profession_token(context);
      return Profession_Owner(new Dungeon_Reference_Profession(name, child, get_source_point()));
    }
    else if (input.current().is(lexicon.lesser_than)) {
      auto token = Profession_Owner(new Token_Profession(name, get_source_point()));
      auto variant = new Dungeon_Variant(std::move(token));
      auto variant_owner = Profession_Owner(variant);
      input.next();
      process_variant_arguments(*variant, context);
      return variant_owner;
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
      return Profession_Owner(new Function_Profession(std::move(profession), std::move(child)));
    }
    return profession;
  }

}