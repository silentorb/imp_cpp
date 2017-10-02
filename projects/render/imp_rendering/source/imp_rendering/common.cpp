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
#include <overworld/schema/Enchantment_With_Arguments.h>

using namespace std;
using namespace overworld;

namespace imp_rendering {

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
    if (expression.get_type() == Expression::Type::self)
      return Reference_Type::pointer;

    return get_reference_type(const_cast<Expression &>(expression).get_node()->get_profession());
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

  const std::string render_parameter(const overworld::Minion &minion, const overworld::Scope &scope) {
    auto &profession = minion.get_profession();
    auto profession_string = render_profession(profession, scope);
    auto last_character = profession_string[profession_string.size() - 1];
    auto separator = last_character == '&' || last_character == '*'
                     ? ""
                     : " ";
//    std::string separator =
//      profession.get_ownership() == Ownership::owner || profession.get_ownership() == Ownership::reference
//      ? " &"
//      : " ";

    return profession_string + separator + minion.get_name();
  }

  const std::string render_function_parameters(const overworld::Function &function) {
    return "(" +
           join(function.get_parameters(), Joiner<overworld::Parameter *>(
             [& function](const overworld::Minion *minion) {
               return render_parameter(*minion, function.get_parent_scope());
             }), ", ") + ")";
  }

  const std::string render_function_return_signature(const overworld::Function &function) {
    string return_text = function.is_constructor()
                         ? ""
                         : render_profession(function.get_profession(), function.get_parent_scope()) + " ";

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

  const std::string render_minion_expression(const overworld::Member_Expression &minion) {
    return sanitize_name(minion.get_member().get_name());
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
    if (parameter.transfers_ownership())
      return "std::move(" + result + ")";

    return render_cast(parameter.get_profession(), argument, result);
  }

  const std::string render_function_call(const overworld::Invoke &function_call, const overworld::Scope &scope) {
    auto minions = function_call.get_signature().get_parameters().begin();
    return render_expression(function_call.get_expression(), scope) + "(" +
           join(function_call.get_arguments(), Joiner<const overworld::Expression_Owner>(
             [& minions, & scope](const overworld::Expression_Owner &expression) {
               return render_argument(*expression, **minions++, scope);
             }), ", ") + ")";
  }

  const std::string
  render_dictionary(const std::map<Minion *, Expression_Owner> &dictionary, const overworld::Scope &scope) {
    return join(dictionary, Map_Joiner<Minion *, const overworld::Expression_Owner>(
      [& scope](Minion *minion, const overworld::Expression_Owner &expression) {
        return minion->get_name() + ": " + render_expression(*expression, scope);
      }), ",\n");;
  }

  const std::string render_instantiation(const overworld::Instantiation &instantiation, const overworld::Scope &scope) {
    return render_profession_owner(instantiation.get_profession(), scope) +
           "(new " + instantiation.get_profession().get_name()
           + "(" + render_dictionary(instantiation.get_dictionary(), scope) + "))";
  }

  const std::string render_operator(overworld::Operator_Type value) {
    static const string operator_strings[] = {
      "=",
    };

    return operator_strings[(int) value];
  }

  const std::string render_assignment(
    const overworld::Assignment &declaration, const overworld::Scope &scope) {
    return render_expression(*declaration.get_target(), scope) + ' '
           + render_operator(declaration.get_operator()) + ' '
           + render_cast(declaration.get_target()->get_node()->get_profession(),
                         declaration.get_value()->get_node()->get_profession(),
                         render_expression(*declaration.get_value(), scope))
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
    if (expression.get_type() == overworld::Expression::Type::block) {
      auto &block = *dynamic_cast<const overworld::Block_Expression *>(&expression);
      return render_block(header, block.get_block());
    }
    else {
      Stroke result(new imp_artisan::internal::Standard_Block(header));
      result << render_statement(expression, scope);
      return result;
    }
  }

  const std::string render_separator(const overworld::Profession &profession) {
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

  std::string render_chain(const overworld::Chain &chain, const overworld::Scope &scope) {
    return render_expression(chain.get_first(), scope)
           + render_separator(chain.get_first().get_node()->get_profession_reference().get_profession())
           + render_expression(chain.get_second(), scope);
  }

  const std::string render_expression(const overworld::Expression &input_expression, const overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case overworld::Expression::Type::literal:
        return render_literal(*dynamic_cast<const overworld::Literal *>(&input_expression));

      case overworld::Expression::Type::member:
        return render_minion_expression(*dynamic_cast<const overworld::Member_Expression *>(&input_expression));

      case overworld::Expression::Type::invoke:
        return render_function_call(
          *dynamic_cast<const overworld::Invoke *>(&input_expression), scope);

      case overworld::Expression::Type::instantiation:
        return render_instantiation(
          *dynamic_cast<const overworld::Instantiation *>(&input_expression), scope);

      case overworld::Expression::Type::self:
        return "this";

      case overworld::Expression::Type::chain:
        return render_chain(*dynamic_cast<const overworld::Chain *>(&input_expression), scope);

      default:
        throw std::runtime_error(" Not implemented.");
    }
  }

  Stroke render_statement(const overworld::Expression &input_expression, const overworld::Scope &scope) {
    switch (input_expression.get_type()) {

      case overworld::Expression::Type::assignment:
        return render_assignment(
          *dynamic_cast<const overworld::Assignment *>(&input_expression), scope);

      case overworld::Expression::Type::block:
        throw "Not Implemented.";
//        return render_block(*dynamic_cast<const overworld::Block *>(&input_expression));

      case overworld::Expression::Type::If:
        return render_if(*dynamic_cast<const overworld::If *>(&input_expression), scope);

      case overworld::Expression::Type::return_nothing:
        return render_return_nothing(*dynamic_cast<const overworld::Return *>(&input_expression)) + ";";

      case overworld::Expression::Type::return_with_value:
        return render_return_with_value(*dynamic_cast<const overworld::Return_With_Value *>(&input_expression), scope);

      case overworld::Expression::Type::variable_declaration:
        return render_variable_declaration(
          *dynamic_cast<const overworld::Minion_Declaration *>(&input_expression), scope);

      case overworld::Expression::Type::variable_declaration_and_assignment:
        return render_variable_declaration_with_assignment(
          *dynamic_cast<const overworld::Minion_Declaration_And_Assignment *>(&input_expression), scope);

      default:
        return render_expression(input_expression, scope) + ";";
        throw std::runtime_error(" Not implemented.");
    }
  }

  void render_statements(Stroke &stroke, const overworld::Expressions &statements, const overworld::Scope &scope) {
    for (auto &statement : statements) {
      stroke << render_statement(*statement, scope);
    }
  }

  const std::string get_cpp_name(const overworld::Dungeon &dungeon) {
    auto &external = Enchantment_Library::get_external_name();
    auto enchantment = dungeon.get_enchantments().get_enchantment(external);
    if (enchantment) {
      auto name = enchantment->get_argument_string(0);
      if (name != "")
        return name;
    }
    return dungeon.get_name();
  }

  const std::string render_primary_dungeon_token(const Dungeon_Interface &dungeon_interface,
                                                 const overworld::Scope &scope) {
    if (dungeon_interface.get_dungeon_type() == Dungeon_Type::variant) {
      auto &variant = dynamic_cast<const Dungeon_Variant &>(dungeon_interface);
      std::string parameter_string = join(variant.get_professions(), Joiner<Profession *>(
        [& scope](const Profession *profession) {
          return render_profession(*profession, scope);
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
    auto parent = dungeon_interface.get_original().get_scope();
//    if (parent == &scope)
//      return result;

    while (parent && parent != &scope) {
      auto parent_dungeon = dynamic_cast<const Dungeon_Interface *>(parent);
      if (parent_dungeon->get_name() == "")
        break;

      result = render_dungeon_interface(*parent_dungeon, scope)
               + "::" + result;

      parent = parent->get_parent();
    }

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