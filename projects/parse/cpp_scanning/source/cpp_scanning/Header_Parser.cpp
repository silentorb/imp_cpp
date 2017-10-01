#include <underworld/schema/Dungeon_Variant.h>
#include "Header_Parser.h"
#include <underworld/schema/Function.h>

using namespace runic;
using namespace underworld;

namespace cpp_scanning {

  underworld::Dungeon &Header_Parser::create_dungeon(const std::string &name, Context &context) {
    auto &current_dungeon = context.get_dungeon();
    auto new_dungeon = new Dungeon(name, &current_dungeon);
    context.get_dungeon().add_profession(std::unique_ptr<Dungeon>(new_dungeon), input.get_source_point());
    return *new_dungeon;
  }

  underworld::Dungeon &Header_Parser::get_or_create_dungeon(const std::string &name, Context &context) {
    auto &current_dungeon = context.get_dungeon();
    auto member = current_dungeon.get_member(name);
    if (member) {
      auto profession_member = dynamic_cast<Profession_Member *>(member);
      if (!profession_member)
        throw Token_Exception<Token>(input.current(), name + " is not a namespace.");

      auto dungeon = dynamic_cast<Dungeon *>(&profession_member->get_profession());
      if (!dungeon)
        throw Token_Exception<Token>(input.current(), name + " is not a namespace.");

      return *dungeon;
    }

    return create_dungeon(name, context);
  }

  void Header_Parser::skip_to(const runic::Whisper &whisper) {
    while (true) {
      if (input.current().is(whisper))
        return;

      if (input.current().is(lexicon.end_of_file))
        throw Token_Exception<Token>(input.current(), "Unexpected end of file.");

      input.next();
    }
  }

  void Header_Parser::skip_line() {
    while (true) {
      if (input.current().is(lexicon.end_of_file))
        return;

      if (input.current().follows_terminator())
        return;

      input.next();
    }
  }

  void Header_Parser::skip_scope() {
    int depth = 1;
    input.next();
    while (true) {
      if (input.current().is(lexicon.left_brace)) {
        ++depth;
      }

      if (input.current().is(lexicon.right_brace)) {
        --depth;
        if (depth == 0)
          break;
      }

      if (input.current().is(lexicon.end_of_file))
        throw Token_Exception<Token>(input.current(), "Unexpected end of file while looking for \"}\".");

      input.next();
    }

    input.next();
  }

  void Header_Parser::process_function_parameters(Virtual_Function &function, Context &context) {
    input.expect(lexicon.left_paren);
    if (input.next().is(lexicon.right_paren))
      return;

    while (true) {
      auto source_point = input.get_source_point();
      auto profession = process_profession(context);
      auto parameter_name = input.current().get_text();
      input.next();
      function.add_parameter(parameter_name, std::move(profession), source_point);

      if (input.current().is_not(lexicon.comma))
        break;

      input.next();
    }

    input.expect(lexicon.right_paren);
    input.next();
  }

  Function & Header_Parser::process_common_function_declaration(Context &context) {
    auto name = input.current().get_text();
    auto function = new Virtual_Function(name, input.get_source_point(), context.get_dungeon());
    context.get_dungeon().add_member(std::unique_ptr<Member>(function));
    input.next();
    process_function_parameters(*function, context);
    return *function;;
  }

  void Header_Parser::process_constructor(Context &context) {
    process_common_function_declaration(context);
    if (input.if_is(lexicon.colon)) {
      skip_to(lexicon.left_brace);
      skip_scope();
    }
  }

  void Header_Parser::process_destructor(Context &context) {

  }

  void Header_Parser::process_variant_arguments(underworld::Dungeon_Variant &variant, Context &context) {
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

  underworld::Profession_Owner Header_Parser::process_profession_token(Context &context) {
    auto name = input.current().get_text();
    auto source_point = input.get_source_point();
    if (input.next().is(lexicon.namespace_delimiter)) {
      if (input.next().is(lexicon.String)) {
        auto primitive = lookup.get_primitive(input.current().get_match().get_type());
        input.next();
        return Profession_Owner(new Primitive(primitive, input.get_source_point()));
      }
      input.expect(lexicon.identifier);
      auto child = process_profession_token(context);
      return Profession_Owner(new Dungeon_Reference_Profession(name, child, source_point));
    }
    else {
      auto token = Profession_Owner(new Token_Profession(name, source_point));
      if (input.current().is(lexicon.lesser_than)) {
        auto variant = new Dungeon_Variant(std::move(token));
        auto variant_owner = Profession_Owner(variant);
        input.next();
        process_variant_arguments(*variant, context);
        return variant_owner;
      }
      else {
        return token;
      }
    }
  }

  underworld::Profession_Owner Header_Parser::process_profession_internal(Context &context) {
    auto primitive = lookup.get_primitive(input.current().get_match().get_type());
    if (primitive != Primitive_Type::Unknown)
      return Profession_Owner(new Primitive(primitive, input.get_source_point()));

    return process_profession_token(context);
  }

  underworld::Profession_Owner Header_Parser::process_profession(Context &context) {
    bool Const = input.if_is(lexicon.Const);

    auto profession = process_profession_internal(context);
    if (input.if_is(lexicon.ampersand)) {
      return Profession_Owner(new Reference(std::move(profession)));
    }

    if (input.if_is(lexicon.asterisk)) {
      return Profession_Owner(new Pointer(std::move(profession)));
    }

    return profession;
  }

  void Header_Parser::process_function_or_variable(Context &context) {
    auto name = input.current().get_text();
    if (context.is_class() && name == context.get_dungeon().get_name() && input.peek().is(lexicon.left_paren)) {
      process_constructor(context);
      return;
    }

    auto profession = process_profession(context);
    auto member_name = input.expect(lexicon.identifier).get_text();
    if (input.peek().is(lexicon.left_paren)) {
      process_common_function_declaration(context);
    }
    else {
      auto minion = new Minion(member_name, std::move(profession), input.get_source_point());
      context.get_dungeon().add_member(std::unique_ptr<Member>(minion));
      input.next();
    }
  }

  void Header_Parser::process_class_or_struct(Context &context, Access_Mode access_mode) {
    auto name = input.expect_next(lexicon.identifier).get_text();
    input.expect_next(lexicon.left_brace);
    if (context.get_dungeon().get_member(name))
      throw Token_Exception<Token>(input.current(), "The symbol " + name + " is already used.");

    auto &namespace_dungeon = create_dungeon(name, context);
    Context new_context(namespace_dungeon, access_mode);
    input.next();
    process_class_namespace(new_context);
  }

  bool Header_Parser::process_class_or_namespace_member(Context &context) {
    if (input.current().is(lexicon.pound_sign)) {
      input.next();
      skip_line();
    }
    else if (input.current().is(lexicon.Class)) {
      process_class_or_struct(context, Access_Mode::Private);
    }
    else if (input.current().is(lexicon.Struct)) {
      process_class_or_struct(context, Access_Mode::Public);
    }
    else if (input.current().is(lexicon.identifier)) {
      process_function_or_variable(context);
    }
    else return false;

    return true;
  }

  void Header_Parser::process_class_member(Context &context) {
    if (input.current().is(lexicon.Private)) {
      input.expect_next(lexicon.colon);
      input.next();
      context.set_access_mode(Access_Mode::Private);
    }
    else if (input.current().is(lexicon.Protected)) {
      input.expect_next(lexicon.colon);
      input.next();
      context.set_access_mode(Access_Mode::Protected);
    }
    else if (input.current().is(lexicon.Public)) {
      input.expect_next(lexicon.colon);
      input.next();
      context.set_access_mode(Access_Mode::Public);
    }
    else if (process_class_or_namespace_member(context)) {
      // All good here.
    }
    else {
      throw runic::Syntax_Exception(input.current());
    }
  }

  void Header_Parser::process_namespace_member(Context &context) {

    if (input.current().is(lexicon.Namespace)) {
      auto name = input.expect_next(lexicon.identifier).get_text();
      input.expect_next(lexicon.left_brace);
      auto &namespace_dungeon = get_or_create_dungeon(name, context);
      Context new_context(namespace_dungeon);
      input.next();
      return process_child_namespace(new_context);
    }

    if (process_class_or_namespace_member(context))
      return;

    throw runic::Syntax_Exception(input.current());
  }

  void Header_Parser::process_class_namespace(Context &context) {
    while (input.current().is_not(lexicon.right_brace)) {
      process_class_member(context);
    }

    input.next();
  }

  void Header_Parser::process_child_namespace(Context &context) {
    while (input.current().is_not(lexicon.right_brace)) {
      process_namespace_member(context);
    }

    input.next();
  }

  void Header_Parser::process_root(Context &context) {
    while (input.current().is_not(lexicon.end_of_file)) {
      process_namespace_member(context);
    }
  }

  void Header_Parser::parse(underworld::Dungeon &root_dungeon) {
    Context context(root_dungeon);
    input.next();
    process_root(context);
  }

}