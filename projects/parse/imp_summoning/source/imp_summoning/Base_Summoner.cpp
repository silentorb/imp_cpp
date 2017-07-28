#include "Base_Summoner.h"
#include "exceptions.h"

using namespace std;
using namespace underworld;

namespace imp_summoning {

  Base_Summoner::Base_Summoner(Stream &input, Lookup &lookup) :
    input(input),
    lookup(lookup),
    lexicon(runic_imp::Lexicon::get_instance().patterns) {

  }

  underworld::Profession_Owner Base_Summoner::process_profession_token(Context &context) {
    auto name = input.current().get_text();
    if (input.peek().is(lexicon.dot)) {
      input.next();
      if (input.next().is_not(lexicon.identifier))
        throw Syntax_Exception(input.current());

      auto child = process_profession_token(context);
      return Profession_Owner(new Dungeon_Reference_Profession(name, child, get_source_point()));
    }
    else {
      return Profession_Owner(new Token_Profession(name, get_source_point()));
    }
  }

  underworld::Profession_Owner Base_Summoner::process_profession(Context &context) {
    if (input.current().is(lexicon.ampersand)) {
      return Profession_Owner(new Reference(new Primitive(Primitive_Type::Unknown, get_source_point())));
    }
    auto primitive = lookup.get_primitive(input.current().get_match().get_type());
    if (primitive != Primitive_Type::Unknown)
      return Profession_Owner(new Primitive(primitive, get_source_point()));

    return process_profession_token(context);

    throw Syntax_Exception(input.current());
  }

//  underworld::Profession_Owner Base_Summoner::parse_profession_path(std::vector<std::string> &path, Context &context) {
//    path.clear();
//    while(input.current().is(lexicon.identifier)) {
//      path.push_back(input.current().get_text());
//      if(input.peek().is_not(lexicon.dot))
//        break;
//
//      input.next();
//      input.next();
//    }
//  }

}