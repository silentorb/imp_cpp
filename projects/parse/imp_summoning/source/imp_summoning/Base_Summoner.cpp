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

  underworld::Profession_Owner Base_Summoner::process_profession_token(const Link &link, Context &context) {
    if (input.current().is(lexicon.dot)) {
      input.expect_next(lexicon.identifier);
      Link child_link = {input.current().get_text(), get_source_point()};
      input.next();
      auto child = process_profession_token(child_link, context);
      return Profession_Owner(new Dungeon_Reference_Profession(link.name, std::move(child), link.source_point));
    }
    else if (input.current().is(lexicon.lesser_than)) {
      auto token = Profession_Owner(new Token_Profession(link.name, link.source_point));
      auto variant = new Dungeon_Variant(std::move(token));
      auto variant_owner = Profession_Owner(variant);
      input.next();
      process_variant_arguments(*variant, context);
      return variant_owner;
    }
    else {
      return Profession_Owner(new Token_Profession(link.name, link.source_point));
    }
  }

  underworld::Profession_Owner Base_Summoner::process_decorator(underworld::Decorator_Type decorator_type,
                                                                Context &context) {
    input.next();
    if (input.current().is(lexicon.identifier)) {
      auto child = process_profession(context);
      return Profession_Owner(new Decorator(decorator_type, std::move(child)));
    }
    else {
      return Profession_Owner(new Decorator(decorator_type, Profession_Owner(new Unknown(get_source_point()))));
    }
  }

  underworld::Profession_Owner Base_Summoner::process_profession_internal(Context &context) {
    if (input.current().is(lexicon.ampersand))
      return process_decorator(Decorator_Type::reference, context);

    if (input.current().is(lexicon.dollar_sign))
      return process_decorator(Decorator_Type::owner, context);

    if (input.current().is(lexicon.question_mark)) {
      input.next();
      return Profession_Owner(new Unknown(get_source_point()));
    }

    auto primitive = lookup.get_primitive(input.current().get_match().get_type());
    if (primitive != Primitive_Type::Unknown) {
      auto result = Profession_Owner(new Primitive(primitive, get_source_point()));
      input.next();
      return result;
    }

    Link link = {
      input.expect(lexicon.identifier).get_text(),
      get_source_point()
    };
    input.next();
    return process_profession_token(link, context);
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