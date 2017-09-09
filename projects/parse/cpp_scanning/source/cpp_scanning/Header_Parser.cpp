#include "Header_Parser.h"

using namespace runic;

namespace cpp_scanning {

  void Header_Parser::skip_line() {
    while (true) {
      if (input.current().is(lexicon.end_of_file))
        return;

      if (input.current().follows_terminator())
        return;

      input.next();
    }
  }

  void Header_Parser::process_namespace_member(Context &context) {
    if (input.current().is(lexicon.pound_sign)) {
      input.next();
      skip_line();
      return;
    }


  }

  void Header_Parser::process_namespace(Context &context) {
    while (input.current().is_not(lexicon.end_of_file)) {
      process_namespace_member(context);
    }
  }

  void Header_Parser::parse() {
    Context context;
    input.next();
    process_namespace(context);
  }

}