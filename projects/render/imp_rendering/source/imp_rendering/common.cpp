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


  const std::string render_minion_with_signature(const overworld::Minion &minion) {
    return render_profession(minion.get_profession()) + ' ' + minion.get_name();
  }

  const std::string render_parameter(const overworld::Minion &minion) {
    auto &profession = minion.get_profession();
    std::string separator = profession.get_ownership() == Ownership::owner
                            ? " &"
                            : " ";

    return render_profession(profession) + separator + minion.get_name();
  }

  const std::string render_function_parameters(const overworld::Function &function) {
    return "(" +
           join(function.get_parameters(), Joiner<overworld::Minion *>(
             [](const overworld::Minion *minion) {
               return render_parameter(*minion);
             }), ", ") + ")";
  }

  const std::string render_function_return_signature(const overworld::Function &function) {
    string return_text = function.is_constructor()
                         ? ""
                         : render_profession(function.get_profession()) + " ";

    return return_text;
  }

  const std::string render_function_declaration(const overworld::Function &function) {
    auto static_clause = function.is_static() ? "static " : "";
    return static_clause + render_function_return_signature(function)
           + function.get_name()
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
    return minion.get_member().get_name();
  }

  const std::string render_return_nothing(const overworld::Return &input_return) {
    return "return;";
  }

  const std::string render_return_with_value(const overworld::Return_With_Value &input_return) {
    return "return " + render_expression(input_return.get_value()) + ";";
  }

  const std::string render_variable_declaration(const overworld::Minion_Declaration &declaration,
                                                const overworld::Scope &scope) {
    return "auto " + declaration.get_minion().get_name();
  }

  const std::string render_function_call(const overworld::Invoke &function_call) {
    return function_call.get_function().get_name() + "(" +
           join(function_call.get_arguments(), Joiner<const overworld::Expression_Owner>(
             [](const overworld::Expression_Owner &expression) {
               return render_expression(*expression);
             }), ", ") + ")";
  }

  const std::string render_instantiation(const overworld::Instantiation &instantiation) {
    return instantiation.get_profession().get_name() + "(" +
           join(instantiation.get_dictionary(), Map_Joiner<Minion *, const overworld::Expression_Owner>(
             [](Minion *minion, const overworld::Expression_Owner &expression) {
               return minion->get_name() + ": " + render_expression(*expression);
             }), ",\n") + ")";
  }

  const std::string render_operator(overworld::Operator_Type value) {
    static const string operator_strings[] = {
      "=",
    };

    return operator_strings[(int) value];
  }

  const std::string render_assignment(
    const overworld::Assignment &declaration, const overworld::Scope &scope) {
    return render_expression(*declaration.get_target()) + ' '
           + render_operator(declaration.get_operator()) + ' '
           + render_expression(*declaration.get_value())
           + ";";
  }

  const std::string render_variable_declaration_with_assignment(
    const overworld::Minion_Declaration_And_Assignment &declaration, const overworld::Scope &scope) {
    return render_variable_declaration(declaration, scope)
           + " = " + render_expression(declaration.get_expression()) + ";";
  }

  Stroke render_if(const overworld::If &input_if, const overworld::Scope &scope) {
    auto header = "if (" + render_expression(input_if.get_condition()) + ")";
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

  std::string render_chain(const overworld::Chain &chain) {
    return render_expression(*chain.get_first())
           + "." + render_expression(*chain.get_first());
  }

  const std::string render_expression(const overworld::Expression &input_expression) {
    switch (input_expression.get_type()) {

      case overworld::Expression::Type::literal:
        return render_literal(*dynamic_cast<const overworld::Literal *>(&input_expression));

      case overworld::Expression::Type::member:
        return render_minion_expression(*dynamic_cast<const overworld::Member_Expression *>(&input_expression));

      case overworld::Expression::Type::invoke:
        return render_function_call(
          *dynamic_cast<const overworld::Invoke *>(&input_expression));

      case overworld::Expression::Type::instantiation:
        return render_instantiation(
          *dynamic_cast<const overworld::Instantiation *>(&input_expression));

      case overworld::Expression::Type::self:
        return "this";

      case overworld::Expression::Type::chain:
        return render_chain(*dynamic_cast<const overworld::Chain *>(&input_expression));

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
        return render_return_with_value(*dynamic_cast<const overworld::Return_With_Value *>(&input_expression));

      case overworld::Expression::Type::variable_declaration:
        return render_variable_declaration(
          *dynamic_cast<const overworld::Minion_Declaration *>(&input_expression), scope);

      case overworld::Expression::Type::variable_declaration_and_assignment:
        return render_variable_declaration_with_assignment(
          *dynamic_cast<const overworld::Minion_Declaration_And_Assignment *>(&input_expression), scope);

      default:
        return render_expression(input_expression) + ";";
        throw std::runtime_error(" Not implemented.");
    }
  }

  void render_statements(Stroke &stroke, const overworld::Expressions &statements, const overworld::Scope &scope) {
    for (auto &statement : statements) {
      stroke << render_statement(*statement, scope);
    }
  }

  const std::string render_profession_internal(const overworld::Profession &profession) {
    auto type = profession.get_type();

    switch (type) {
      case overworld::Profession_Type::primitive: {
        auto index = dynamic_cast<const overworld::Primitive *>(&profession)->get_primitive_type();
        return primitive_names[(int) index];
      }
      case overworld::Profession_Type::dungeon: {
        auto &dungeon = *dynamic_cast<const overworld::Dungeon *>(&profession);
        return dungeon.get_name();
      }

      case overworld::Profession_Type::Void:
        return "void";

      default:
        return "unknown";
    }
//    throw std::runtime_error(" Not implemented.");
  }

  const std::string render_profession(const overworld::Profession &profession) {
    auto result = render_profession_internal(profession);
    if (profession.get_ownership() == Ownership::owner)
      return "std::unique_ptr<" + result + ">";

    return result;
  }

//  const std::string render_profession_as_owner(const overworld::Profession &profession) {
//    return "std::unique_ptr<" + render_profession(profession) + ">";
//  }
//
//  const std::string render_profession_as_owner_reference(const overworld::Profession &profession) {
//    return render_profession_as_owner(profession) + " &";
//  }

  Stroke render_function_definition(const overworld::Function &function) {
    auto function_signature = render_function_return_signature(function)
                              + function.get_name()
                              + render_function_parameters(function);

    return render_block(function_signature, function.get_block());
  }


  Stroke render_includes(const std::vector<File *> &files) {
    Stroke result;
    for (auto file : files) {
      result << wrap("#include <" + file->get_filename() + ">");
    }
    return result;
  }
}