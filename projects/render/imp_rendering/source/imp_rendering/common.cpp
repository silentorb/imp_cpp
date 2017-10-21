#include "common.h"
#include <overworld/schema/Function.h>
#include <overworld/expressions/Flow_Control.h>
#include <overworld/expressions/Return.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/expressions/Literal.h>
#include <overworld/expressions/Block_Expression.h>
#include <overworld/expressions/Assignment.h>
#include <overworld/expressions/Member_Expression.h>
#include <overworld/expressions/Invoke.h>
#include <overworld/expressions/Chain.h>
#include <overworld/expressions/Instantiation.h>
#include <overworld/expressions/Lambda.h>
#include <cpp_stl/Standard_Library.h>
#include <overworld/expressions/Range.h>

using namespace std;
using namespace overworld;

namespace imp_rendering {

  static const cpp_stl::Standard_Library *standard_library;
  static const cpp_stl::Standard_Library::Enchantments *standard_enchantments;

  void set_standard_library(cpp_stl::Standard_Library &library) {
    standard_library = &library;
    standard_enchantments = &library.get_enchantments();
  }

  static const std::string primitive_names[] = {
    "bool",
    "double",
    "float",
    "int",
    "std::string",
    "void"
  };

  enum class Reference_Type {
      none,
      pointer,
      reference
  };

  Reference_Type get_reference_type(const Profession &profession) {
    if (profession.get_type() == Profession_Type::reference) {
      auto &reference = dynamic_cast<const Reference &>(profession);
      return reference.is_pointer()
             ? Reference_Type::pointer
             : Reference_Type::reference;
    }
    return Reference_Type::none;
  }

  Reference_Type get_reference_type(const Expression &expression) {
    if (expression.get_type() == Expression_Type::self)
      return Reference_Type::pointer;

    auto &profession = const_cast<Expression &>(expression).get_node()->get_profession();
    return get_reference_type(*profession);
  }

  template<typename A, typename B>
  const std::string render_cast(const A &target, const B &source, const std::string &text) {
    auto target_type = get_reference_type(target);
    auto source_type = get_reference_type(source);

    if (target_type == Reference_Type::reference && source_type == Reference_Type::pointer)
      return "*" + text;

    return text;
  }

  const std::string render_reference_symbol(const Reference &reference) {
    return reference.is_pointer() ? "*" : "&";
  }

  Stroke render_block(const std::string &header, const overworld::Block &block) {
    Stroke result(new imp_artisan::internal::Standard_Block(header));
    render_statements(result, block.get_expressions(), block.get_scope());
    return result;
  }

  Stroke render_shrinking_block(const std::string &header, const overworld::Block_Expression &block) {
    Stroke result(new imp_artisan::internal::Standard_Block(header));
    render_statements(result, block.get_expressions(), block.get_scope());
    return result;
  }

  const std::string render_minion_with_signature(const overworld::Minion &minion, const overworld::Scope &scope) {
    auto signature = render_profession(minion.get_profession(), scope);
    auto last_character = signature[signature.size() - 1];
    string separator = last_character == '*' || last_character == '&'
                       ? ""
                       : " ";

    return signature + separator + minion.get_name();
  }

  const std::string render_parameter(const overworld::Parameter &parameter, const overworld::Scope &scope) {
    auto &profession = parameter.get_profession();
    auto profession_string = render_profession(profession, scope);
    auto last_character = profession_string[profession_string.size() - 1];
    auto separator = last_character == '&' || last_character == '*'
                     ? ""
                     : " ";
//    std::string separator =
//      profession.get_ownership() == Ownership::owner || profession.get_ownership() == Ownership::reference
//      ? " &"
//      : " ";

    return profession_string + separator + parameter.get_name();
  }

  const std::string render_function_parameters(const overworld::Function &function) {
    return "(" +
           join(function.get_signature().get_parameters(), Joiner<overworld::Parameter *>(
             [& function](const overworld::Parameter *minion) {
               return render_parameter(*minion, function.get_parent_scope());
             }), ", ") + ")";
  }

  const std::string render_function_return_signature(const overworld::Function &function) {
    string return_text = function.is_constructor()
                         ? ""
                         : render_profession(function.get_signature().get_last().get_profession(),
                                             function.get_parent_scope()) + " ";

    return return_text;
  }

  const std::string render_function_declaration(const overworld::Function &function) {
    auto static_clause = function.has_enchantment(Enchantment_Library::get_static()) ? "static " : "";
    return static_clause + render_function_return_signature(function)
           + sanitize_name(function.get_name())
           + render_function_parameters(function);
  }

  const std::string render_literal(const overworld::Literal &literal) {

    switch (literal.get_primitive_type()) {

      case overworld::Primitive_Type::Bool:
        return (*dynamic_cast<const overworld::Literal_Bool *>(&literal)).get_value()
               ? "true"
               : "false";

      case overworld::Primitive_Type::Int:
        return to_string((*dynamic_cast<const overworld::Literal_Int *>(&literal)).get_value());

      case overworld::Primitive_Type::String:
        return "\"" + (*dynamic_cast<const overworld::Literal_String *>(&literal)).get_value() + "\"";

      default:
        throw std::runtime_error("Not implemented.");
    }
  }

  const std::string render_member_expression(const overworld::Member_Expression &member_expression) {
    auto &member = member_expression.get_member();
    if (member.get_type() == Member_Type::function) {
      auto &function = member.get_function();
      return get_cpp_name(function);
    }
    else if (member.get_type() == Member_Type::minion) {
      auto &minion = member.get_minion();
      return get_cpp_name(minion);
    }
    else if (member.get_type() == Member_Type::dungeon) {
      auto &dungeon = member.get_dungeon();
      return get_cpp_name(dungeon);
    }

    throw std::runtime_error("Not supported.");
  }

  const std::string render_return_nothing(const overworld::Return &input_return) {
    return "return;";
  }

  const std::string render_return_with_value(const overworld::Return_With_Value &input_return,
                                             const overworld::Scope &scope) {
    return "return " + render_expression(input_return.get_value(), scope) + ";";
  }

  const std::string render_variable_declaration(const overworld::Minion_Declaration &declaration,
                                                const overworld::Scope &scope) {
    auto &minion = declaration.get_minion();
    auto &profession = minion.get_profession();
    auto reference_symbol = profession.get_type() == Profession_Type::reference
                            ? render_reference_symbol(*dynamic_cast<const Reference *>(&profession))
                            : "";

    return "auto " + reference_symbol + minion.get_name();
  }

  const std::string render_argument(const overworld::Expression &argument, const overworld::Parameter &parameter,
                                    const overworld::Scope &scope) {
    auto result = render_expression(argument, scope);
    if (parameter.get_profession().get_ownership() == Ownership::owner)
      return "std::move(" + result + ")";

    return render_cast(parameter.get_profession(), argument, result);
  }

  Stroke render_for_loop(const overworld::Invoke &function_call, const overworld::Scope &scope) {
    auto &parameters = function_call.get_signature().get_parameters();
    auto minions = parameters.begin();
    auto *member_expression = dynamic_cast<const Member_Expression *>(&function_call.get_expression().get_last());
    auto &member = member_expression->get_member();
    auto &function = member.get_function();
    auto container = render_expression(function_call.get_expression(), scope);
    container = container.substr(0, container.size() - 4);
    auto lambda = dynamic_cast<Lambda *>(function_call.get_arguments()[0].get());
    auto header =
      "for (auto &" + lambda->get_function().get_signature().get_parameters()[0]->get_name() + " : " + container + ")";
    return render_block(header, lambda->get_function().get_block());
  }

  Stroke render_function_call(const overworld::Invoke &function_call, const overworld::Scope &scope) {
    auto minions = function_call.get_signature().get_parameters().begin();
    auto *member_expression = dynamic_cast<const Member_Expression *>(&function_call.get_expression().get_last());
    auto &member = member_expression->get_member();
    if (member.get_type() == Member_Type::function) {
      auto &function = member.get_function();
      auto &enchantments = function.get_enchantments();
      if (enchantments.has_enchantment(*standard_enchantments->input_stream)) {
        return render_expression(function_call.get_expression(), scope) + " << " +
               join(function_call.get_arguments(), Joiner<const overworld::Expression_Owner>(
                 [& minions, & scope](const overworld::Expression_Owner &expression) {
                   return render_argument(*expression, **minions++, scope);
                 }), " << ");
      }
    }

    return render_expression(function_call.get_expression(), scope) + "(" +
           join(function_call.get_arguments(), Joiner<const overworld::Expression_Owner>(
             [& minions, & scope](const overworld::Expression_Owner &expression) {
               return render_argument(*expression, **minions++, scope);
             }), ", ") + ")";
  }

  const std::string render_dictionary(const std::map<Minion *, Expression_Owner> &dictionary,
                                      const overworld::Scope &scope) {
    return join(dictionary, Map_Joiner<Minion *, const overworld::Expression_Owner>(
      [& scope](Minion *minion, const overworld::Expression_Owner &expression) {
        return minion->get_name() + ": " + render_expression(*expression, scope);
      }), ",\n");;
  }

  const std::string render_primary_dungeon_token(const Dungeon_Interface &dungeon_interface,
                                                 const overworld::Scope &scope) {
    if (dungeon_interface.get_dungeon_type() == Dungeon_Type::variant) {
      auto &variant = dynamic_cast<const Dungeon_Variant &>(dungeon_interface);
      std::string parameter_string = join(variant.get_arguments(), Joiner<const Generic_Argument_Owner &>(
        [& scope](const Generic_Argument_Owner &argument) {
          return render_profession(*argument->get_node().get_profession(), scope);
        }), ", ");

      return get_cpp_name(variant.get_original()) + "<" + parameter_string + ">";
    }
    else {
      auto &dungeon = dynamic_cast<const Dungeon &>(dungeon_interface);
      return get_cpp_name(dungeon);
    }
  }

  const std::string render_dungeon_interface(const Dungeon_Interface &dungeon_interface,
                                             const overworld::Scope &scope) {
    auto result = render_primary_dungeon_token(dungeon_interface, scope);
    auto &parent = dungeon_interface.get_original().get_scope();
//    if (parent == &scope)
//      return result;

    throw std::runtime_error("Needs updating.");
//    while (parent && parent != &scope) {
//      auto parent_dungeon = dynamic_cast<const Dungeon_Interface *>(parent);
//      if (parent_dungeon->get_original().get_name() == "")
//        break;
//
//      result = render_dungeon_interface(*parent_dungeon, scope)
//               + "::" + result;
//
//      parent = parent->get_parent_scope();
//    }

    return result;
  }

  const std::string render_profession_internal(const overworld::Profession &profession, const Scope &scope) {
    auto type = profession.get_type();

    switch (type) {
      case overworld::Profession_Type::primitive: {
        auto index = dynamic_cast<const overworld::Primitive *>(&profession)->get_primitive_type();
        return primitive_names[(int) index];
      }
      case overworld::Profession_Type::dungeon:
      case overworld::Profession_Type::variant: {
        auto &dungeon_interface = dynamic_cast<const Dungeon_Interface &>(profession);
        return render_dungeon_interface(dungeon_interface, scope);
      }

      case overworld::Profession_Type::Void:
        return "void";

      case Profession_Type::reference: {
        auto &reference = *dynamic_cast<const Reference *>(&profession);
        return render_profession_internal(reference.get_profession(), scope) + " " + render_reference_symbol(reference);
      }

      case overworld::Profession_Type::generic_parameter:
        return profession.get_name();

      default:
        return "!unknown!";
    }
//    throw std::runtime_error(" Not implemented.");
  }

  const std::string render_instantiation(const overworld::Instantiation &instantiation, const overworld::Scope &scope) {
    auto &profession = instantiation.get_profession();
    auto core = render_profession_internal(profession, scope) + "("
                + render_dictionary(instantiation.get_dictionary(), scope) + ")";

    if (profession.get_ownership() == Ownership::value) {
      return core;
    }

    return render_profession_owner(profession, scope) + "(new " + core + ")";
  }

  const std::string render_operator(overworld::Operator_Type value) {
    static const string operator_strings[] = {
      "=",
    };

    return operator_strings[(int) value];
  }

  const std::string render_assignment(const overworld::Assignment &declaration, const overworld::Scope &scope) {
    auto &target = declaration.get_target();
    auto &value = declaration.get_value();

    return render_expression(*target, scope) + ' '
           + render_operator(declaration.get_operator()) + ' '
           + render_cast(*target->get_node()->get_profession(),
                         *value->get_node()->get_profession(),
                         render_expression(*value, scope))
           + ";";
  }

  const std::string render_variable_declaration_with_assignment(
    const overworld::Minion_Declaration_And_Assignment &declaration, const overworld::Scope &scope) {
    return render_variable_declaration(declaration, scope)
           + " = " + render_expression(declaration.get_expression(), scope) + ";";
  }

  Stroke render_if(const overworld::If &input_if, const overworld::Scope &scope) {
    auto header = "if (" + render_expression(input_if.get_condition(), scope) + ")";
    auto &expression = input_if.get_expression();
    if (expression.get_type() == overworld::Expression_Type::block) {
      auto &block = *dynamic_cast<const overworld::Block_Expression *>(&expression);
      return render_block(header, block.get_block());
    }
    else {
      Stroke result(new imp_artisan::internal::Standard_Block(header));
      result << render_statement(expression, scope);
      return result;
    }
  }

  const std::string render_separator(const Expression &expression) {
    if (expression.get_type() == Expression_Type::member) {
      auto member_expression = static_cast<const Member_Expression *>(&expression);
      auto member_expression2 = dynamic_cast<const Member_Expression *>(&expression);
      auto &member = member_expression->get_member();
      if (member.get_type() == Member_Type::dungeon)
        return "::";
    }

    auto &profession = expression.get_node()->get_profession();
    switch (profession.get_ownership()) {
      case Ownership::owner:
        return "->";

      default:
        return ".";
    }
  }

  const std::string render_cast(const overworld::Profession &target, const overworld::Profession &source,
                                const std::string &value) {
    if (target.get_type() == Profession_Type::reference && source.get_type() == Profession_Type::reference) {
      auto &target_reference = dynamic_cast<const Reference &>(target);
      auto &source_reference = dynamic_cast<const Reference &>(source);
      if (target_reference.is_pointer()) {
        if (!source_reference.is_pointer()) {
          return "&" + value;
        }
      }
    }
    return value;
  }

  std::string render_lambda(const overworld::Lambda &lambda, const overworld::Scope &scope) {
    auto &function = lambda.get_function();
    auto token = render_block("", function.get_block());
    auto block = token.render("");
    return "[&] " + render_function_parameters(function)
           + block;
  }

  std::string render_chain(const overworld::Chain &chain, const overworld::Scope &scope) {
    return render_expression(chain.get_first(), scope)
           + render_separator(chain.get_first())
           + render_expression(chain.get_second(), scope);
  }

  const std::string render_range(const Range &range) {
    if (range.get_start().get_type() == Range_Value_Type::absolute
        && range.get_end().get_type() == Range_Value_Type::none) {
      return "[" + std::to_string(range.get_start().get_value()) + "]";
    }
    else {
      throw std::runtime_error("Not implemented.");
    }
  }

  const std::string render_expression(const overworld::Expression &input_expression, const overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case overworld::Expression_Type::literal:
        return render_literal(*dynamic_cast<const overworld::Literal *>(&input_expression));

      case overworld::Expression_Type::member:
        return render_member_expression(*dynamic_cast<const overworld::Member_Expression *>(&input_expression));

      case overworld::Expression_Type::invoke:
        return render_function_call(
          *dynamic_cast<const overworld::Invoke *>(&input_expression), scope).render("");

      case overworld::Expression_Type::instantiation:
        return render_instantiation(
          *dynamic_cast<const overworld::Instantiation *>(&input_expression), scope);

      case overworld::Expression_Type::self:
        return "this";

      case overworld::Expression_Type::chain:
        return render_chain(*dynamic_cast<const overworld::Chain *>(&input_expression), scope);

      case overworld::Expression_Type::lambda:
        return render_lambda(static_cast<const overworld::Lambda &>(input_expression), scope);
      default:
        throw std::runtime_error(" Not implemented.");
    }
  }

  Stroke render_statement(const overworld::Expression &input_expression, const overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case overworld::Expression_Type::assignment:
        return render_assignment(
          *dynamic_cast<const overworld::Assignment *>(&input_expression), scope);

      case overworld::Expression_Type::block:
        throw "Not Implemented.";
//        return render_block(*dynamic_cast<const overworld::Block *>(&input_expression));

      case overworld::Expression_Type::If:
        return render_if(*dynamic_cast<const overworld::If *>(&input_expression), scope);

      case overworld::Expression_Type::return_nothing:
        return render_return_nothing(*dynamic_cast<const overworld::Return *>(&input_expression)) + ";";

      case overworld::Expression_Type::return_with_value:
        return render_return_with_value(*dynamic_cast<const overworld::Return_With_Value *>(&input_expression), scope);

      case overworld::Expression_Type::variable_declaration:
        return render_variable_declaration(
          *dynamic_cast<const overworld::Minion_Declaration *>(&input_expression), scope);

      case overworld::Expression_Type::variable_declaration_and_assignment:
        return render_variable_declaration_with_assignment(
          *dynamic_cast<const overworld::Minion_Declaration_And_Assignment *>(&input_expression), scope);

      case overworld::Expression_Type::invoke: {
        auto &function_call = *dynamic_cast<const overworld::Invoke *>(&input_expression);
        auto *member_expression = dynamic_cast<const Member_Expression *>(&function_call.get_expression().get_last());
        auto &member = member_expression->get_member();
        if (member.get_type() == Member_Type::function) {
          auto &function = member.get_function();
          auto &enchantments = function.get_enchantments();
          if (enchantments.has_enchantment(*standard_enchantments->functional.map)) {
            return render_for_loop(function_call, scope);
          }
        }
      }

      default:
        return render_expression(input_expression, scope) + ";";
//        throw std::runtime_error(" Not implemented.");
    }
  }

  void render_statements(Stroke &stroke, const overworld::Expressions &statements, const overworld::Scope &scope) {
    for (auto &statement : statements) {
      stroke << render_statement(*statement, scope);
    }
  }

  template<typename T>
  const std::string _get_cpp_name(const T &dungeon) {
    auto &external = Enchantment_Library::get_external_name();
    auto enchantment = dungeon.get_enchantments().get_enchantment(external);
    if (enchantment) {
      auto name = enchantment->get_argument_string(0);
      if (name != "")
        return name;
    }
    return sanitize_name(dungeon.get_name());
  }

  const std::string get_cpp_name(const overworld::Dungeon &dungeon) {
    return _get_cpp_name(dungeon);
  }

  const std::string get_cpp_name(const overworld::Minion &minion) {
    return _get_cpp_name(minion);
  }

  const std::string get_cpp_name(const overworld::Function &function) {
    return _get_cpp_name(function);
  }

  const std::string render_profession_owner(const overworld::Profession &profession, const Scope &scope) {
    return "std::unique_ptr<" + render_profession_internal(profession, scope) + ">";
  }

  const std::string render_profession(const overworld::Profession &profession, const Scope &scope) {
    if (profession.get_ownership() == Ownership::owner)
      return render_profession_owner(profession, scope);

    return render_profession_internal(profession, scope);
  }

//  const std::string render_profession_as_owner(const overworld::Profession &profession) {
//    return "std::unique_ptr<" + render_profession(profession) + ">";
//  }
//
//  const std::string render_profession_as_owner_reference(const overworld::Profession &profession) {
//    return render_profession_as_owner(profession) + " &";
//  }

  std::string sanitize_name(const string &name) {
    if (name == "new")
      return "_new";

    return name;
  }

  std::string render_template_prefix(const std::vector<overworld::Generic_Parameter *> &generic_parameters) {
    std::string parameter_string = join(generic_parameters, Joiner<Generic_Parameter *>(
      [](const overworld::Generic_Parameter *parameter) {
        return "typename " + parameter->get_name();
      }), ", ");

    return "template <" + parameter_string + ">";
  }

  Stroke render_possible_generic_block(const std::vector<overworld::Generic_Parameter *> &generic_parameters,
                                       Stroke stroke) {
    if (generic_parameters.size() > 0) {
      return Stroke(new imp_artisan::Tight_Group(
        {
          render_template_prefix(generic_parameters),
          std::move(stroke)
        }));
    }
    else {
      return stroke;
    }
  }

  Stroke render_function_definition(const overworld::Function &function) {
    auto static_clause = function.is_inline() && function.has_enchantment(Enchantment_Library::get_static())
                         ? "static "
                         : "";

    auto function_signature = static_clause + render_function_return_signature(function)
                              + sanitize_name(function.get_name())
                              + render_function_parameters(function);

    auto with_block = static_cast<const overworld::Function_With_Block *>(&function);
    return render_possible_generic_block(function.get_generic_parameters(),
                                         render_block(function_signature, with_block->get_block())
    );
  }

  Stroke render_includes(const std::vector<File_Reference> &files) {
    Stroke result;
    for (auto &file : files) {
      if (file.is_local()) {
        result << "#include \"" + file.get_filename() + "\"";
      }
      else {
        result << "#include <" + file.get_filename() + ">";
      }
    }
    return result;
  }
}