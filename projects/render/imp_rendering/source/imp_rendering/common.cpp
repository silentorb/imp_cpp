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
#include <overworld/schema/Dungeon_Reference.h>

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

  Reference_Type get_reference_type(const Profession_Reference &profession) {
    if (profession.get_type() == Profession_Type::reference) {
      auto &reference = dynamic_cast<const Reference &>(*profession);
      return reference.get_reference_type();
    }
    return Reference_Type::none;
  }

  Reference_Type get_reference_type(const Expression &expression) {
    if (expression.get_type() == Expression_Type::self)
      return Reference_Type::pointer;

    auto &profession = const_cast<Expression &>(expression).get_node()->get_profession();
    return get_reference_type(profession);
  }

  template<typename A, typename B>
  const std::string render_cast(const A &target, const B &source, const std::string &text) {
    auto target_type = get_reference_type(target);
    auto source_type = get_reference_type(source);
    if (target.get_ownership() == Ownership::owner)
      return "std::move(" + text + ")";

    if (target_type == Reference_Type::reference && source_type == Reference_Type::pointer)
      return "*" + text;

    return text;
  }

  const std::string render_reference_symbol(const Reference &reference) {
    return std::string(1, reference.get_symbol());
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

  const std::string render_profession_with_spacing(const overworld::Profession_Reference &profession,
                                                   const Scope &scope) {
    auto result = render_profession(profession, scope);
    auto last_character = result[result.size() - 1];
    return last_character == '*' || last_character == '&'
           ? result
           : result + " ";
  }

  const std::string render_minion_with_signature(const overworld::Minion &minion, const overworld::Scope &scope) {
    auto signature = render_profession_with_spacing(minion.get_profession(), scope);
    return signature + minion.get_name();
  }

  const std::string render_parameter(const overworld::Parameter &parameter, const overworld::Scope &scope) {
    auto &profession = parameter.get_profession();
    auto profession_string = render_profession_with_spacing(profession, scope);
//    std::string separator =
//      profession.get_ownership() == Ownership::owner || profession.get_ownership() == Ownership::reference
//      ? " &"
//      : " ";

    return profession_string + parameter.get_name();
  }

  const std::string render_function_parameters(const overworld::Function &function, const Scope &scope) {
    return "(" +
           join(function.get_signature().get_parameters(), Joiner<overworld::Parameter *>(
             [& function, &scope](const overworld::Parameter *minion) {
               return render_parameter(*minion, scope);
             }), ", ") + ")";
  }

  const std::string render_function_return_signature(const overworld::Function &function, const overworld::Scope &scope) {
    string return_text = function.is_constructor(scope.get_owner())
                         ? ""
                         : render_profession_with_spacing(function.get_signature().get_last().get_profession(),
                                                          scope);

    return return_text;
  }

  const std::string render_function_declaration(const overworld::Function &function, const overworld::Scope &scope) {
    auto static_clause = function.has_enchantment(Enchantment_Library::get_static()) ? "static " : "";
    return static_clause + render_function_return_signature(function, scope)
           + sanitize_name(function.get_name())
           + render_function_parameters(function, scope);
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
    switch (member.get_type()) {
      case Member_Type::function: {
        auto &function = member.get_function();
        return get_cpp_name(function);
      }
      case Member_Type::minion: {
        auto &minion = member.get_minion();
        return get_cpp_name(minion);
      }
      case Member_Type::dungeon: {
        auto &dungeon = member.get_dungeon();
        return get_cpp_name(dungeon);
      }
      case Member_Type::parameter: {
        auto &parameter = member.get_parameter();
        return get_cpp_name(parameter);
      }
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
                            ? render_reference_symbol(*dynamic_cast<const Reference *>(profession.get()))
                            : "";

    return "auto " + reference_symbol + minion.get_name();
  }

  const std::string render_argument(const overworld::Expression &argument, const overworld::Parameter &parameter,
                                    const overworld::Scope &scope) {
    auto result = render_expression(argument, scope);
    return render_cast(parameter.get_profession(), argument, result);
  }

  Stroke render_for_loop(const overworld::Invoke &function_call, const overworld::Scope &scope) {
    auto &parameters = function_call.get_function().get_signature().get_parameters();
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
    auto minions = function_call.get_function().get_signature().get_elements().begin();
    auto *member_expression = dynamic_cast<const Member_Expression *>(&function_call.get_expression().get_last());
    auto &member = member_expression->get_member();
    if (member.get_type() == Member_Type::function) {
      auto &function = member.get_function();
      auto &enchantments = function.get_enchantments();
      int i = 0;
      if (enchantments.has_enchantment(*standard_enchantments->input_stream)) {
        return render_expression(function_call.get_expression(), scope) + " << " +
               join(function_call.get_arguments(), Joiner<const overworld::Expression_Owner>(
                 [& minions, & scope, &function_call, &i](const overworld::Expression_Owner &expression) {
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

  const std::string render_primary_dungeon_token(const Dungeon &dungeon_interface,
                                                 const overworld::Scope &scope) {
    if (dungeon_interface.is_generic()) {
      auto &variant = dynamic_cast<const Dungeon &>(dungeon_interface);
      std::string parameter_string = join(variant.get_arguments(), Joiner<const Generic_Argument_Owner &>(
        [& scope](const Generic_Argument_Owner &argument) {
          return render_profession(argument->get_node().get_profession(), scope);
        }), ", ");

      return get_cpp_name(variant) + "<" + parameter_string + ">";
    }
    else {
      auto &dungeon = dynamic_cast<const Dungeon &>(dungeon_interface);
      return get_cpp_name(dungeon);
    }
  }

  const std::string render_dungeon_interface(const Dungeon &dungeon_interface,
                                             const overworld::Scope &scope) {
    auto result = render_primary_dungeon_token(dungeon_interface, scope);
    auto &target_parent = dungeon_interface.get_scope().get_parent_scope()->get_owner();
    auto local_parent_scope = scope.get_parent_scope();
    if (local_parent_scope) {
      auto &local_parent = local_parent_scope->get_owner();
      if (!local_parent.is_descendant_of(target_parent)) {
        return render_namespace(target_parent, &local_parent, "::") + "::" + result;
      }
    }

    return result;
  }

  const std::string render_profession_internal(const overworld::Profession_Reference &profession, const Scope &scope) {
    auto type = profession.get_type();

    switch (type) {
      case overworld::Profession_Type::primitive: {
        auto index = dynamic_cast<const overworld::Primitive *>(profession.get())->get_primitive_type();
        return primitive_names[(int) index];
      }
      case overworld::Profession_Type::dungeon: {
        auto dungeon_reference = static_cast<const Dungeon_Reference *>(profession.get());
        return render_dungeon_interface(dungeon_reference->get_dungeon(), scope);
      }

      case overworld::Profession_Type::Void:
        return "void";

      case Profession_Type::reference: {
        auto &reference = *dynamic_cast<const Reference *>(profession.get());
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

    if (profession.get_ownership() == Ownership::owner) {
      return render_profession_owner(profession, scope) + "(new " + core + ")";
    }

    return core;
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

    return render_expression(target, scope) + ' '
           + render_operator(declaration.get_operator()) + ' '
           + render_cast(*target.get_node()->get_profession(),
                         *value.get_node()->get_profession(),
                         render_expression(value, scope))
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

    auto &profession = expression.get_profession();
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
      if (target_reference.get_reference_type() == Reference_Type::pointer) {
        if (source_reference.get_reference_type() == Reference_Type::owner) {
          return value + ".get()";
        }
        else if (source_reference.get_reference_type() != Reference_Type::pointer) {
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
    return "[] " + render_function_parameters(function, scope)
           + block;
  }

  std::string render_chain(const overworld::Chain &chain, const overworld::Scope &scope) {
    std::string result = "";
    auto &links = chain.get_expressions();
    auto last = links[links.size() - 1].get();

    for (auto &link : links) {
      result += render_expression(*link, scope);
      if (link.get() != last)
        result += render_separator(*link);
    }

    return result;
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
    return _get_cpp_name(dungeon.get_base_original());
  }

  const std::string get_cpp_name(const overworld::Minion &minion) {
    return _get_cpp_name(minion);
  }

  const std::string get_cpp_name(const overworld::Function &function) {
    return _get_cpp_name(function);
  }

  const std::string get_cpp_name(const overworld::Parameter &parameter) {
    return sanitize_name(parameter.get_name());
  }

  const std::string render_namespace(const Parent &parent, const overworld::Parent *current,
                                     const std::string &delimiter) {
    if (parent.get_type() == Parent_Type::dungeon) {
      if (parent.get_name() != "") {
        auto parent2 = parent.get_parent();
        if (parent2 && (!current || !current->is_descendant_of(*parent2))) {
          return render_namespace(*parent2, current, delimiter) + delimiter +
                 get_cpp_name(parent2->get_dungeon());
        }
        else {
          return get_cpp_name(parent.get_dungeon());
        }
      }
    }

    return "";
  }

  const std::string render_profession_owner(const overworld::Profession_Reference &profession, const Scope &scope) {
    return "std::unique_ptr<" + render_profession_internal(profession, scope) + ">";
  }

  const std::string render_profession(const overworld::Profession_Reference &profession, const Scope &scope) {
    if (profession.get_ownership() == Ownership::owner)
      return render_profession_owner(profession, scope);

    std::string decorator = "";
    if (profession.get_ownership() == Ownership::pointer)
      decorator = " *";
    else if (profession.get_ownership() == Ownership::reference)
      decorator = " &";

    return render_profession_internal(profession, scope) + decorator;
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

  class Namespace_Block : public imp_artisan::internal::Standard_Block {
  public:
      explicit Namespace_Block(const string &header) : Standard_Block(header) {}

      std::string render(const imp_artisan::Indent &indent) const {
        return render_main(
          indent, 1,
          header + " {\n\n",
          "\n" + indent + "}"
        );
      }
  };

  Stroke render_possible_namespace_block(const overworld::Parent &parent, Stroke stroke) {
    auto namespace_string = render_namespace(parent, nullptr, "::");
    if (namespace_string == "") {
      return stroke;
    }
    else {
      Stroke result{new Namespace_Block(
        "namespace " + namespace_string
      )};
      result.add(stroke);
      return result;
    }
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

  Stroke render_function_definition(const overworld::Function &function, const overworld::Scope &scope) {
    auto static_clause = function.is_inline() && function.has_enchantment(Enchantment_Library::get_static())
                         ? "static "
                         : "";

    auto function_signature = static_clause + render_function_return_signature(function, scope)
                              + sanitize_name(function.get_name())
                              + render_function_parameters(function, scope);

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