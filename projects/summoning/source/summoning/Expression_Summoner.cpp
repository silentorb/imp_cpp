#include <underworld/expressions/Literal.h>
#include "Expression_Summoner.h"
#include "exceptions.h"

using namespace underworld;

namespace summoning {

  Expression_Summoner::Expression_Summoner(Stream &input, Profession_Library &profession_library) :
    Base_Summoner(input, profession_library) {}

  void Expression_Summoner::process_variable_declaration(Context &context) {
    input.expect_next(lexicon.identifier);
    context.get_scope().create_portal(input.current().get_text(), profession_library.get_unknown());
    if (input.next().is(lexicon.assignment)) {
      process_expression(context);
    }
  }

  void Expression_Summoner::process_statements(underworld::Block &block, Context &context) {
    input.next();

    while (!input.current().is(lexicon.right_brace)) {
      if (input.current().is(lexicon.Var)) {
        process_variable_declaration(context);
      }
      else {
        throw Syntax_Exception(input.current());
      }
    }
  }

  Expression_Owner Expression_Summoner::process_expression(Context &context) {
    input.next();

    if (input.current().is(lexicon.literal_int)) {
      int value = std::stoi(input.current().get_text());
      return Expression_Owner(new Literal_Int(value));
    }
    else {
      throw Syntax_Exception(input.current());
    }
  }

}
