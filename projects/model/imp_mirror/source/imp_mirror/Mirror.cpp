
#include <overworld/expressions/Block_Expression.h>
#include <overworld/expressions/Flow_Control.h>
#include <overworld/expressions/Literal.h>
#include <overworld/expressions/Return.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/schema/Function.h>
#include <overworld/expressions/Member_Expression.h>
#include <overworld/expressions/Operator.h>
#include <underworld/expressions/Assignment.h>
#include <overworld/expressions/Assignment.h>
#include <overworld/expressions/Self.h>
#include <overworld/expressions/Chain.h>
#include <overworld/expressions/Instantiation.h>
#include <overworld/schema/Temporary_Interface.h>
#include <underworld/schema/Enchantment.h>
#include <overworld/expressions/Lambda.h>
#include <overworld/expressions/Range.h>
#include <overworld/schema/Dungeon_Reference.h>
#include <underworld/schema/Dungeon_Variant.h>
#include "Mirror.h"
#include "Code_Error.h"

namespace imp_mirror {

  template<typename T>
  void process_external(T &member, overworld::Enchantment_Library &enchantment_library,
                        overworld::File_Library &header_files) {
    auto external = member.get_enchantments().get_enchantment(enchantment_library.get_external());

    if (external) {
      auto value = external->get_argument_string(0);
      if (value != "") {
        auto &file = header_files.count(value)
                     ? header_files[value]
                     : header_files[value] = overworld::File(value, true);
        member.set_file(&file);
      }
    }
  }

  class Basic_Profession_Setter : public overworld::Profession_Setter {
  public:
      void set_profession(overworld::Node &node, overworld::Profession_Reference &profession) override {
        node.set_profession(profession, *this);
      }
  };

  static Basic_Profession_Setter profession_setter;

  void Mirror::connect(overworld::Node &first, overworld::Node &second) {
    if (graph)
      graph->connect(first, second);
  }

  overworld::Dungeon *Mirror::find_enchantment_dungeon(const underworld::Profession &input_profession,
                                                       Scope &scope) {
    if (input_profession.get_type() == underworld::Profession_Type::token) {
      auto token = static_cast<const underworld::Token_Profession *>(&input_profession);
      auto dungeon = profession_library.get_enchantment_library().find_enchantment(token->get_name());
      if (dungeon)
        return dungeon;
    }

    auto reflection = reflect_profession(input_profession, scope);
    auto &output_profession = reflection.profession;

    if (output_profession.get_type() != overworld::Profession_Type::dungeon) {
      throw Code_Error(output_profession.get_name()
                       + " is not a dungeon and cannot be used as an enchantment.",
                       input_profession.get_source_point());
    }

    return &output_profession->get_dungeon_interface();
  }

  void Mirror::reflect_enchantments(const underworld::Enchantment_Array &input_enchantments,
                                    overworld::Enchantment_Container &output_enchantments,
                                    Scope &scope) {
    for (auto &input_enchantment: input_enchantments) {
      auto dungeon = find_enchantment_dungeon(input_enchantment->get_profession(), scope);
      if (!dungeon) {
        find_enchantment_dungeon(input_enchantment->get_profession(), scope);
        throw std::runtime_error("Could not find enchantment for profession "
                                 + input_enchantment->get_profession().get_name());
      }
      if (input_enchantment->get_arguments().size() > 0) {
        auto complex_enchantment = new overworld::Enchantment(*dungeon);
        output_enchantments.add_enchantment(overworld::Enchantment_Owner(complex_enchantment));
        for (auto &argument : input_enchantment->get_arguments()) {
          complex_enchantment->add_argument(reflect_expression(*argument, scope));
        }
      }
      else {
        output_enchantments.add_enchantment(*dungeon);
      }
    }
  }

  void Mirror::apply_node_assignment(overworld::Node &target, overworld::Node &value) {
    connect(value, target);

    // Optimization to reduce the amount of graph solving later on
    // since so often variables types are defined by assigning them an instantiation.
    auto &value_profession = value.get_profession();
    if (target.get_profession().get_type()
        == overworld::Profession_Type::unknown
        && value_profession.get_type()
           != overworld::Profession_Type::unknown) {
      target.set_profession(value_profession,
                            overworld::Empty_Profession_Setter::get_instance());
    }
  }

  overworld::Expression_Owner Mirror::reflect_assignment(const underworld::Assignment &input_assignment,
                                                         Scope &scope) {
    auto target = reflect_expression(*input_assignment.get_target(), scope);
    auto &last = target->get_last();
    auto operator_type = reflect_operator(input_assignment.get_operator());
    auto value = reflect_expression(*input_assignment.get_value(), scope);

    apply_node_assignment(*last.get_node(), *value->get_node());
    connect(*value->get_node(), *last.get_node());
    return overworld::Expression_Owner(new overworld::Assignment(target, operator_type, value));
  }

  overworld::Expression_Owner Mirror::reflect_lambda(const underworld::Lambda &input_lambda,
                                                     Scope &scope) {
    auto &input_function = input_lambda.get_function();
    auto output_function = new overworld::Function_With_Block(input_function.get_name(),
                                                              scope.get_overworld_scope(),
                                                              scope.get_overworld_scope().get_owner(),
                                                              input_function.get_source_point());
    auto output_function_owner = overworld::Function_With_Block_Owner(output_function);
//    scope.add_function(std::unique_ptr<overworld::Function>(output_function));

    Scope block_scope(output_function->get_block().get_scope(), scope);
    reflect_scope1(input_function.get_block().get_scope(),
                   block_scope);

    reflect_enchantments(input_function.get_enchantments(), output_function->get_enchantments(), scope);

    reflect_function_with_block2(input_function, *output_function, block_scope);
    reflect_function_with_block3(input_function, *output_function, block_scope);

    auto lambda = new overworld::Lambda(std::move((output_function_owner)),
                                        scope.get_overworld_scope().get_owner());

    connector.connect_lambda_parameters(*lambda->get_node(), output_function->get_signature());

    return overworld::Expression_Owner(lambda);
  }

  overworld::Expression_Owner Mirror::reflect_literal(const underworld::Literal &input_literal,
                                                      overworld::Parent parent) {

    overworld::Literal *expression;
    switch (input_literal.get_primitive_type()) {

      case underworld::Primitive_Type::Bool:
        expression = new overworld::Literal_Bool(
          (*dynamic_cast<const underworld::Literal_Bool *>(&input_literal)).get_value(), parent,
          input_literal.get_source_point());
        break;

      case underworld::Primitive_Type::Int:
        expression = new overworld::Literal_Int(
          (*(const underworld::Literal_Int *) &input_literal).get_value(),
          parent, input_literal.get_source_point());
        break;

      case underworld::Primitive_Type::String:
        expression = new overworld::Literal_String(
          (*dynamic_cast<const underworld::Literal_String *>(&input_literal)).get_value(), parent,
          input_literal.get_source_point());
        break;

      default:
        throw std::runtime_error("Not implemented.");
    }

    return overworld::Expression_Owner(expression);
  }

  overworld::Expression_Owner Mirror::reflect_member(const underworld::Member &input_member,
                                                     Scope &scope) {
    auto name = input_member.get_name();
    auto member = scope.find_member(name);
    if (!member)
      throw Code_Error("Unknown symbol " + name, input_member.get_source_point());

    auto result = new overworld::Member_Expression(*member, input_member.get_source_point());
//    graph->connect(*result->get_node(), overworld::get_member_node(*member));
    return overworld::Expression_Owner(result);
  }

  overworld::Expression_Owner Mirror::reflect_member_expression(
    const underworld::Member_Expression &input_member_expression,
    Scope &scope) {
    auto name = input_member_expression.get_name();
    auto member = scope.find_member(name);
    if (!member)
      throw Code_Error("Unknown symbol " + name, input_member_expression.get_source_point());

    auto result = new overworld::Member_Expression(*member, input_member_expression.get_source_point());
//    graph->connect(*result->get_node(), overworld::get_member_node(*member));
    return overworld::Expression_Owner(result);
  }

  overworld::Operator_Type Mirror::reflect_operator(const underworld::Operator &input_operator) {
    return (overworld::Operator_Type) (int) input_operator.get_operator_type();
  }

  overworld::Expression_Owner Mirror::reflect_return_nothing(const underworld::Return &input_return) {
    return overworld::Expression_Owner(new overworld::Return());
  }

  overworld::Expression_Owner Mirror::reflect_return_with_value(const underworld::Return_With_Value &input_return,
                                                                Scope &scope) {
    auto expression = reflect_expression(input_return.get_value(), scope);
    auto &first = scope.get_overworld_scope().get_owner().get_function()
      .get_signature().get_last().get_node();
    connect(*expression->get_node(), first);
    return overworld::Expression_Owner(
      new overworld::Return_With_Value(std::move(expression)));
  }

  overworld::Expression_Owner
  Mirror::reflect_variable_declaration(const underworld::Minion_Declaration &input_declaration,
                                       Scope &scope) {
    auto &variable = scope.get_overworld_scope().get_minion(input_declaration.get_minion().get_name());
    return overworld::Expression_Owner(new overworld::Minion_Declaration(variable));
  }

  overworld::Expression_Owner Mirror::reflect_variable_declaration_with_assignment(
    const underworld::Minion_Declaration_And_Assignment &input_declaration, Scope &scope) {
    auto &input_minion = input_declaration.get_minion();
//    auto profession = reflect_member_expression(input_minion, scope);
//    auto member = scope.find_member(input_minion.get_name());
//    if (!member)
//      throw Code_Error("Unknown symbol " + input_minion.get_name(), input_minion.get_source_point());

    auto variable = new overworld::Minion(input_minion.get_name(),
                                          overworld::Profession_Library::get_unknown(),
                                          overworld::Parent(scope.get_overworld_scope().get_owner().get_function()),
                                          input_minion.get_source_point());
    scope.get_overworld_scope().add_minion(overworld::Minion_Owner(variable));
    auto expression = reflect_expression(input_declaration.get_expression(), scope);
    apply_node_assignment(variable->get_node(), *expression->get_node());
    return overworld::Expression_Owner(new overworld::Minion_Declaration_And_Assignment(*variable, expression));
  }

  overworld::Expression_Owner Mirror::reflect_if(const underworld::If &input_if,
                                                 Scope &scope) {
    return overworld::Expression_Owner(new overworld::If(
      reflect_expression(input_if.get_condition(), scope),
      reflect_statement(input_if.get_expression(), scope)
    ));
  }

  void Mirror::reflect_block(const underworld::Block &input_block, overworld::Block &output_block, Scope &scope) {
    for (auto &input_expression : input_block.get_expressions()) {
      auto output_expression = reflect_statement(*input_expression, scope);
      output_block.add_expression(output_expression);
    }
  }

  overworld::Expression_Owner Mirror::reflect_block_expression(const underworld::Block &input_block,
                                                               Scope &scope) {
    auto output_block = new overworld::Block_Expression(&scope.get_overworld_scope());
    auto result = overworld::Expression_Owner(output_block);
    reflect_block(input_block, output_block->get_block(), scope);
    return result;
  }

  overworld::Expression_Owner Mirror::reflect_invoke(const underworld::Invoke &function_call,
                                                     Scope &scope) {
    auto &input_expression = function_call.get_expression();
    auto output_expression = reflect_expression(input_expression, scope);

    auto &source_arguments = function_call.get_arguments();
    std::vector<overworld::Expression_Owner> arguments;
    arguments.reserve(source_arguments.size());
    for (auto &source_argument : source_arguments) {
      arguments.push_back(reflect_expression(*source_argument, scope));
    }

    auto invoke = new overworld::Invoke(output_expression, arguments,
                                        function_call.get_source_point());
    auto result = overworld::Expression_Owner(invoke);
//    auto parameters = invoke->get_function().get_signature().get_elements().begin();

//    for (auto &source_argument : invoke->get_arguments()) {
//      auto &parameter = *parameters++;
//      if (parameter->has_enchantment(overworld::Enchantment_Library::get_container_assignment())) {
//        auto container = overworld::find_member_container(invoke->get_expression());
//        auto &variant = container->get_profession().get()->get_dungeon_interface();
////        auto &parameter = create_node(variant.get_arguments()[0]->get_node());
////        auto &parameter = create_node(Overworld_Element(element->get_node()));
//        connect(argument, parameter);
//      }
//    }
    connector.connect_parameters_and_arguments(*invoke, source_arguments, scope);

    return result;
  }

  overworld::Range_Value reflect_range_value(const underworld::Range_Value &input_range_value) {
    auto type = static_cast<overworld::Range_Value_Type >(input_range_value.get_type());
    return overworld::Range_Value(type, input_range_value.get_value());
  }

  overworld::Expression_Owner Mirror::reflect_range(const underworld::Range &input_range, Scope &scope) {
    return overworld::Expression_Owner(new overworld::Range(
      reflect_range_value(input_range.get_start()),
      reflect_range_value(input_range.get_end())
    ));
  }

  using Prepared_Profession_Tuple = std::tuple<overworld::Profession_Reference, overworld::Dungeon *>;

//  Prepared_Profession_Tuple prepare_profession(overworld::Profession_Reference &initial_profession) {
//    if (initial_profession.get_type() == overworld::Profession_Type::dungeon) {
//      auto dungeon_reference = dynamic_cast<overworld::Dungeon_Reference *>(initial_profession.get());
//      auto &dungeon = dungeon_reference->get_dungeon();
//      for (auto &parameter: dungeon.get_generic_parameters()) {
//        if (parameter->get_type() == overworld::Profession_Type::generic_parameter) {
//          std::vector<overworld::Profession_Reference> professions;
//          professions.push_back(overworld::Profession_Library::get_unknown());
//          auto variant = new overworld::Dungeon(dungeon, professions);
//          auto dungeon_reference2 = new overworld::Dungeon_Reference(overworld::Dungeon_Owner(variant));
//          return Prepared_Profession_Tuple(
//            overworld::Profession_Reference(dungeon_reference2),
//            variant
//          );
//        }
//      }
//    }

//    return Prepared_Profession_Tuple(
//      initial_profession, nullptr);
//  }

  bool is_value(overworld::Profession &profession) {
    if (profession.get_type() == overworld::Profession_Type::dungeon) {
      auto &dungeon = profession.get_dungeon_interface();
      return dungeon.has_enchantment(overworld::Enchantment_Library::get_value());
    }

    return false;
  }

  overworld::Expression_Owner Mirror::reflect_instantiation(const underworld::Instantiation &instantiation,
                                                            Scope &scope) {
    auto &input_profession_expression = instantiation.get_profession_expression();
    auto reflection = reflect_profession(input_profession_expression, scope);
    auto &output_profession = reflection.profession;

    if (output_profession.get_type() == overworld::Profession_Type::unknown)
      throw Code_Error("Could not instantiate type " + output_profession.get_name(), instantiation.get_source_point());

    auto &source_arguments = instantiation.get_dictionary();
    auto output_instantiation = new overworld::Instantiation(output_profession,
                                                             scope.get_overworld_scope().get_owner(),
                                                             instantiation.get_source_point());
    overworld::Expression_Owner result(output_instantiation);

    auto &attributes = reflection.attributes;
    output_instantiation->get_node()->set_storage(attributes.storage);
    auto ownership = attributes.storage == overworld::Storage_Type::value || is_value(*reflection.profession)
                     ? overworld::Ownership::copy
                     : overworld::Ownership::implicit_move;

    output_instantiation->get_node()->set_ownership(ownership);

    for (auto &pair : source_arguments) {
      auto name = pair.first;
      auto input_member = scope.find_member(name);
      if (!input_member)
        throw Code_Error("Unknown symbol " + name, instantiation.get_source_point());

      auto &minion = input_member->get_minion();
      auto value = reflect_expression(*pair.second, scope);
      connect(*value->get_node(), minion.get_node());
      output_instantiation->add_expression(minion, std::move(value));
    }

    return result;
  }

  overworld::Expression_Owner Mirror::reflect_chain_member(overworld::Expression &first,
                                                           const underworld::Expression &second,
                                                           Scope &scope) {
    auto &previous_expression = first.get_last();
    auto &profession = previous_expression.get_profession();
    if (second.get_type() == underworld::Expression_Type::member) {
      auto &member_expression = cast<underworld::Member_Expression>(second);
      if (profession.get_type() == overworld::Profession_Type::dungeon) {
        auto dungeon = get_dungeon_interface(first.get_last());
        auto member = dungeon->get_member_or_null(member_expression.get_name());
        if (!member) {
          throw Code_Error("Could not find member `" + member_expression.get_name() + "`",
                           member_expression.get_source_point());
        }

        auto result = new overworld::Member_Expression(*member, second.get_source_point());
//        graph->connect(*result->get_node(), overworld::get_member_node(*member));
        return overworld::Expression_Owner(result);
      }
      else if (profession.get_type() == overworld::Profession_Type::unknown) {
        if (previous_expression.get_type() == overworld::Expression_Type::member) {
          auto previous_member_expression = dynamic_cast<overworld::Member_Expression *>(&previous_expression);
          auto interface = new overworld::Temporary_Interface();
//          auto interface = std::make_shared<overworld::Temporary_Interface>();
          auto pointer = std::shared_ptr<overworld::Profession>(interface);
          auto parent_profession = overworld::Profession_Reference(std::move(pointer));
          auto child_member = interface->create_temporary_member(*previous_member_expression,
                                                                 member_expression.get_name(),
                                                                 member_expression.get_source_point(),
                                                                 *graph,
                                                                 scope.get_overworld_scope());

          previous_expression.get_node()->set_profession(parent_profession, profession_setter);

//          auto &first = *previous_expression.get_node();
//          auto &second = *child_member->get_node();
//          auto connection = new overworld::Container_To_Member(first, second, member_expression.get_name());
//          graph->connect(first, second, std::unique_ptr<overworld::Connection>(connection));

          return child_member;
        }
      }

      throw std::runtime_error("Not implemented.");
    }
    else if (second.get_type() == underworld::Expression_Type::chain) {

      auto &chain = cast<underworld::Chain>(second);
      return reflect_chain(chain, scope);
    }

    throw std::runtime_error("Not implemented.");
  }

  overworld::Expression_Owner Mirror::reflect_chain(const underworld::Chain &input_chain, Scope &scope) {
    auto &links = input_chain.get_expressions();
    auto first = reflect_expression(*links[0], scope);
    auto previous = first.get();
    auto chain = new overworld::Chain(scope.get_overworld_scope().get_owner(),
                                      input_chain.get_source_point());
    overworld::Expression_Owner result(chain);
    chain->add_expression(std::move(first));

    for (auto it = links.begin() + 1; it != links.end(); ++it) {
      auto &input_link = *it;
      auto link = reflect_chain_member(*previous, *input_link, scope);
      previous = link.get();
      chain->add_expression(std::move(link));
    }
//    auto second = reflect_chain_member(*first, input_chain.get_second(), scope);
//    auto chain = new overworld::Chain(first, second,
//                                      scope.get_overworld_scope().get_owner(),
//                                      input_chain.get_source_point());
    return result;
  }

  overworld::Expression_Owner Mirror::reflect_expression(const underworld::Expression &input_expression,
                                                         Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression_Type::literal:
        return reflect_literal(*dynamic_cast<const underworld::Literal *>(&input_expression),
                               scope.get_overworld_scope().get_owner());

      case underworld::Expression_Type::member:
        return reflect_member_expression(
          *dynamic_cast<const underworld::Member_Expression *>(&input_expression), scope);

      case underworld::Expression_Type::Operator:
        return overworld::Expression_Owner(new overworld::Operator(
          reflect_operator(*dynamic_cast<const underworld::Operator *>(&input_expression)),
          input_expression.get_source_point()
        ));

      case underworld::Expression_Type::self:
        return overworld::Expression_Owner(new overworld::Self(
          scope.get_overworld_scope().get_owner().get_dungeon(),
          input_expression.get_source_point()));

      case underworld::Expression_Type::chain:
        return reflect_chain(*dynamic_cast<const underworld::Chain *>(&input_expression), scope);

      case underworld::Expression_Type::range:
        return reflect_range(static_cast<const underworld::Range &>(input_expression), scope);

      case underworld::Expression_Type::lambda:
        return reflect_lambda(static_cast<const underworld::Lambda &>(input_expression), scope);
      default:
        return reflect_statement_expression(input_expression, scope);
    }
  }

  overworld::Expression_Owner Mirror::reflect_statement_expression(const underworld::Expression &input_expression,
                                                                   Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression_Type::invoke:
        return reflect_invoke(*dynamic_cast<const underworld::Invoke *>(&input_expression),
                              scope);

      case underworld::Expression_Type::instantiation:
        return reflect_instantiation(*dynamic_cast<const underworld::Instantiation *>(&input_expression),
                                     scope);

      default:
        throw std::runtime_error(" Not implemented.");
    }
  }

  overworld::Expression_Owner Mirror::reflect_statement(const underworld::Expression &input_expression,
                                                        Scope &scope) {
    switch (input_expression.get_type()) {

      case underworld::Expression_Type::assignment:
        return reflect_assignment(*dynamic_cast<const underworld::Assignment *>(&input_expression), scope);

      case underworld::Expression_Type::block:
        return reflect_block_expression(cast<const underworld::Block>(input_expression), scope);

      case underworld::Expression_Type::If:
        return reflect_if(*dynamic_cast<const underworld::If *>(&input_expression), scope);

      case underworld::Expression_Type::return_nothing:
        return reflect_return_nothing(*dynamic_cast<const underworld::Return *>(&input_expression));

      case underworld::Expression_Type::return_with_value:
        return reflect_return_with_value(*dynamic_cast<const underworld::Return_With_Value *>(&input_expression),
                                         scope);

      case underworld::Expression_Type::variable_declaration:
        return reflect_variable_declaration(
          *dynamic_cast<const underworld::Minion_Declaration *>(&input_expression), scope);

      case underworld::Expression_Type::variable_declaration_and_assignment:
        return reflect_variable_declaration_with_assignment(
          *dynamic_cast<const underworld::Minion_Declaration_And_Assignment *>(&input_expression), scope);

      default:
        return reflect_statement_expression(input_expression, scope);
    }
  }

  overworld::Profession_Reference Mirror::reflect_primitive(const underworld::Primitive &primitive) {
    const auto index = (int) primitive.get_primitive_type();
    auto primitive_type = (overworld::Primitive_Type) index;
    return profession_library.get_primitive(primitive_type);
  }

  overworld::Member &reflect_dungeon_member(const underworld::Profession &profession,
                                            Scope &scope) {
    auto input_dungeon = dynamic_cast<const underworld::Token_Profession *>(&profession);
    auto name = input_dungeon->get_name();
    auto member = scope.find_member(name);
    if (!member)
      throw Code_Error("Could not find " + name, input_dungeon->get_source_point());

    return *member;
  }

  overworld::Profession_Reference Mirror::reflect_dungeon_reference(const underworld::Profession &profession,
                                                                    Scope &scope) {
    auto input_dungeon = dynamic_cast<const underworld::Token_Profession *>(&profession);
    auto member = reflect_dungeon_member(profession, scope);
    if (input_dungeon->get_child()) {
      return reflect_profession_child(member, *input_dungeon->get_child(), scope);
    }
    else {
      return get_member_profession_reference2(member);
    }
  }

  overworld::Profession_Reference Mirror::reflect_dungeon_variant(const underworld::Dungeon_Variant &input_variant,
                                                                  Scope &scope) {
    auto &member = reflect_dungeon_member(input_variant.get_original(), scope);
    auto &original = member.get_dungeon();
    auto &input_arguments = input_variant.get_arguments();

    auto output_variant = new overworld::Dungeon(original);
    auto dungeon_reference2 = new overworld::Dungeon_Reference(overworld::Dungeon_Owner(output_variant));
    for (auto &input_argument : input_arguments) {
      auto reflection = reflect_profession(*input_argument, scope);
      output_variant->add_generic_argument(reflection.profession, reflection.attributes);
    }
    return overworld::Profession_Reference(dungeon_reference2);
  }

  overworld::Profession_Reference Mirror::reflect_profession_child(overworld::Member &member,
                                                                   const underworld::Profession &profession,
                                                                   Scope &scope) {
    if (member.get_type() == overworld::Member_Type::dungeon) {
      auto &dungeon = get_dungeon(member);
      auto child = dungeon.get_scope().get_member_or_null(profession.get_name());
      if (!child)
        throw Code_Error("Could not find " + profession.get_name(), profession.get_source_point());

//      if (profession.get_type() == underworld::Profession_Type::dungeon_reference) {
//        Scope dungeon_scope(dungeon.get_scope(), scope);
//        return reflect_dungeon_reference(profession, dungeon_scope);
//      }
      else if (profession.get_type() == underworld::Profession_Type::token) {
        auto input_dungeon = dynamic_cast<const underworld::Token_Profession *>(&profession);
        if (input_dungeon && input_dungeon->get_child()) {
          return reflect_profession_child(*child, *input_dungeon->get_child(), scope);
        }
        else {
          return get_possible_generic_dungeon(get_dungeon(*child));
        }
      }
      else if (profession.get_type() == underworld::Profession_Type::variant) {
        Scope dungeon_scope(dungeon.get_scope(), scope);
//         reflect_dungeon_reference(profession, dungeon_scope);
        auto input_variant = dynamic_cast<const underworld::Dungeon_Variant *>(&profession);
        return reflect_dungeon_variant(*input_variant, dungeon_scope);

      }
      throw std::runtime_error("Not implemented");
    }

    throw std::runtime_error("Not implemented");
  }

  overworld::Profession_Reference Mirror::get_possible_generic_dungeon(overworld::Dungeon &dungeon) {
    auto &generic_parameters = dungeon.get_generic_parameters();
    if (generic_parameters.size() > 0) {
      std::vector<overworld::Profession_Reference> professions;
      professions.reserve(generic_parameters.size());
      for (auto parameter : generic_parameters) {
        professions.push_back(profession_library.get_unknown());
      }
      if (!graph)
        throw std::runtime_error("Should not be here without a graph.");

      return profession_library.get_or_create_dungeon_variant(dungeon, professions, *graph);
    }

    return dungeon.get_reference();
  }

  overworld::Profession_Reference Mirror::reflect_dungeon_usage(const underworld::Token_Profession &token,
                                                                Scope &scope) {
    auto member = scope.find_member(token.get_name());
    if (!member)
      throw Code_Error("Could not find " + token.get_name(), token.get_source_point());

    if (member->get_type() == overworld::Member_Type::dungeon) {
      auto &dungeon = member->get_dungeon();
      return get_possible_generic_dungeon(dungeon);
    }
    else if (member->get_type() == overworld::Member_Type::profession_reference) {
      return get_member_profession_reference(*member);
    }
    else {
      throw new Code_Error(token.get_name() + " is not a dungeon.", token.get_source_point());
    }
  }

  overworld::Profession_Reference Mirror::reflect_function_signature(
    const underworld::Function_Profession &input_signature, Scope &scope) {

    auto signature = new overworld::Function_Signature();
    auto result = overworld::Profession_Reference(signature);
    auto &elements = input_signature.get_elements();
    for (auto &element : elements) {
      auto reflection = reflect_profession(element.get(), scope);
      signature->add_element(overworld::Parameter_Owner(new overworld::Parameter(
        element->get_name(),
        reflection.profession,
        scope.get_overworld_scope().get_owner(), input_signature.get_source_point()
      )));
    }
    return result;
  }

  void Mirror::reflect_function_signature(const underworld::Function_Signature &input_signature,
                                          overworld::Function_Signature &function_profession,
                                          Scope &scope, Scope &function_scope) {
    auto &elements = input_signature.get_elements();
    if (elements.size() < 1)
      throw std::runtime_error("Function signatures requires at least 2 elements.");

    for (auto &element : elements) {
      auto reflection = reflect_profession(element->get_profession(), scope);
      auto parameter = new overworld::Parameter(
        element->get_name(),
        reflection.profession,
        scope.get_overworld_scope().get_owner(), element->get_source_point()
      );
      function_profession.add_element(overworld::Parameter_Owner(parameter));
      if (element->get_name() != "")
        function_scope.get_overworld_scope().add_member(parameter->get_name(),
                                                        overworld::Member(*parameter));
    }
  }

  overworld::Ownership map_reference_decorator(underworld::Decorator_Type type) {
    switch (type) {
      case underworld::Decorator_Type::reference:
        return overworld::Ownership::reference;

      case underworld::Decorator_Type::owner:
        return overworld::Ownership::owner;

//      case underworld::Decorator_Type::pointer:
//        return overworld::Ownership::pointer;

      default:
        throw std::runtime_error("Not supported.");
    }
  }

  Profession_Reflection Mirror::reflect_profession(const underworld::Profession &profession, Scope &scope) {
    switch (profession.get_type()) {
      case underworld::Profession_Type::primitive:
        return {reflect_primitive(cast<underworld::Primitive>(profession)), overworld::Ownership::copy};

      case underworld::Profession_Type::token: {
        auto result = reflect_dungeon_reference(profession, scope);
        return {result, result->get_dungeon_interface().get_default_storage()};
      }

      case underworld::Profession_Type::unknown:
        return profession_library.get_unknown();

      case underworld::Profession_Type::decorator: {
        auto &decorator = cast<underworld::Decorator>(profession);
        auto &input_child_profession = decorator.get_profession();
        auto output_child_profession = reflect_profession(input_child_profession, scope);
        auto type = map_reference_decorator(decorator.get_decorator_type());
        return {output_child_profession.profession, type};
      }

      case underworld::Profession_Type::function: {
        auto signature = dynamic_cast<const underworld::Function_Profession *>(&profession);
        return reflect_function_signature(*signature, scope);
      }
    }

    throw std::runtime_error("Not implemented");
  }

  Profession_Reflection Mirror::reflect_profession(const underworld::Profession *profession, Scope &scope) {
    if (!profession)
      return profession_library.get_unknown();

    return reflect_profession(*profession, scope);
  }

  void Mirror::reflect_function_with_block2(const underworld::Function_With_Block &input_function,
                                            overworld::Function_With_Block &output_function, Scope &scope) {
//    auto profession = reflect_profession(input_function.get_profession(), scope);
//                                          *output_function.get_scope().get_parent());
    Scope block_scope(output_function.get_block().get_scope(), scope);
    reflect_scope2(
      input_function.get_block().get_scope(),
      block_scope);

    reflect_function_signature(input_function.get_signature(), output_function.get_signature(), scope, block_scope);
  }

  void Mirror::reflect_function_with_block3(const underworld::Function_With_Block &input_function,
                                            overworld::Function_With_Block &output_function, Scope &scope) {
    reflect_block(input_function.get_block(), output_function.get_block(), scope);
    output_function.finalize(profession_library);
  }

  void Mirror::reflect_function1(const underworld::Member &member, Scope &scope) {
    auto &input_function = *(dynamic_cast<const underworld::Function *>(&member));
    auto input_function_with_block = (dynamic_cast<const underworld::Function_With_Block *>(&member));
    if (input_function_with_block) {
      auto output_function = new overworld::Function_With_Block(member.get_name(),
                                                                scope.get_overworld_scope(),
                                                                scope.get_overworld_scope().get_owner().get_dungeon(),
                                                                member.get_source_point());
      scope.get_overworld_scope().add_function(std::unique_ptr<overworld::Function>(output_function));

      Scope block_scope(output_function->get_block().get_scope(), scope);
      reflect_scope1(input_function_with_block->get_block().get_scope(),
                     block_scope);

      element_map.add(&input_function, output_function);
    }
    else {
      auto output_function = new overworld::Virtual_Function(member.get_name(),
                                                             scope.get_overworld_scope().get_owner().get_dungeon(),
                                                             member.get_source_point());
      scope.get_overworld_scope().add_function(std::unique_ptr<overworld::Function>(output_function));
//      output_function->set_is_static(input_function.is_static());
//      reflect_enchantments(input_function.get_enchantments(), output_function->get_enchantments(), scope);
      element_map.add(&input_function, output_function);
    }
  }

  void Mirror::reflect_function2(const underworld::Function &input_function, Scope &scope) {
    auto input_function_with_block = dynamic_cast<const underworld::Function_With_Block *>(&input_function);
    if (input_function_with_block) {
      auto output_function_with_block = element_map.find_or_null<overworld::Function_With_Block>(&input_function);
      reflect_enchantments(input_function.get_enchantments(), output_function_with_block->get_enchantments(),
                           scope);
      auto &enchantment_library = profession_library.get_enchantment_library();
      process_external(*output_function_with_block, enchantment_library, header_files);
      Scope child_scope(output_function_with_block->get_scope(), scope);
      reflect_function_with_block2(*input_function_with_block, *output_function_with_block, child_scope);
    }
    else {
      auto virtual_input_function = dynamic_cast<const underworld::Virtual_Function *>(&input_function);
      auto output_function = element_map.find_or_null<overworld::Virtual_Function>(&input_function);
      reflect_enchantments(input_function.get_enchantments(), output_function->get_enchantments(),
                           scope);
      auto &enchantment_library = profession_library.get_enchantment_library();
      process_external(*output_function, enchantment_library, header_files);
      for (auto &source_parameter : virtual_input_function->get_parameters()) {
        auto minion = create_parameter(*source_parameter, scope, *output_function);
        output_function->get_signature().add_element(std::move(minion));
      }
    }
  }

  void Mirror::reflect_function3(const underworld::Function &input_function, Scope &scope) {
    auto input_function_with_block = (dynamic_cast<const underworld::Function_With_Block *>(&input_function));
    if (input_function_with_block) {
      auto &output_function = *element_map.find_or_null<overworld::Function_With_Block>(&input_function);
      Scope child_scope(output_function.get_scope(), scope);
      reflect_function_with_block3(*input_function_with_block, output_function, child_scope);
    }
  }

  std::unique_ptr<overworld::Parameter> Mirror::create_parameter(const underworld::Minion &input_minion,
                                                                 Scope &scope,
                                                                 overworld::Function &function) {
    auto reflection = reflect_profession(input_minion.get_profession(), scope);
    auto &profession = reflection.profession;
    auto parameter = new overworld::Parameter(input_minion.get_name(), profession, overworld::Parent(function),
                                              input_minion.get_source_point());
    auto result = overworld::Parameter_Owner(parameter);
    reflect_enchantments(input_minion.get_enchantments(), parameter->get_enchantments(), scope);
    return result;
  }

  std::unique_ptr<overworld::Minion> Mirror::create_minion(const underworld::Minion &input_minion,
                                                           Scope &scope) {
//    if (input_minion.is_parameter()) {
//      return create_parameter(input_minion, scope, *scope.get_overworld_scope().get_function());
//    }
    auto reflection = reflect_profession(input_minion.get_profession(), scope);
    auto profession = reflection.profession;
//    if (reflection.ownership == overworld::Ownership::reference) {
//      profession = overworld::Profession_Reference(profession->get_base(profession), overworld::Ownership::reference,
//                                                   overworld::Storage_Type::pointer);
//    }
//    else {
//
//    }
//      profession.set_ownership(overworld::Ownership::unknown);

//    auto &function = scope.get_overworld_scope().get_parent().get_function();
    auto minion = std::unique_ptr<overworld::Minion>(
      new overworld::Minion(input_minion.get_name(), profession, scope.get_overworld_scope().get_owner(),
                            input_minion.get_source_point())
    );

    minion->get_node().set_attributes(reflection.attributes);
    return minion;
  }

  overworld::Minion &Mirror::reflect_minion(const underworld::Minion &input_minion, Scope &output_scope) {
    auto output_minion = create_minion(input_minion, output_scope);
    element_map.add(&input_minion, output_minion.get());
    reflect_enchantments(input_minion.get_enchantments(), output_minion->get_enchantments(), output_scope);
    auto &enchantment_library = profession_library.get_enchantment_library();
    process_external(*output_minion, enchantment_library, header_files);
    auto &result = *output_minion;
    output_scope.get_overworld_scope().add_minion(std::move(output_minion));
    return result;
  }

  void Mirror::reflect_dungeon1(const underworld::Dungeon &input_dungeon, Scope &output_scope) {
    overworld::Dungeon *output_dungeon;
    auto existing_member = output_scope.get_overworld_scope().get_member_or_null(input_dungeon.get_name());
    if (existing_member) {
      output_dungeon = &existing_member->get_dungeon();
      auto dungeon = element_map.find_or_null<overworld::Dungeon>(&input_dungeon);
      if (!dungeon)
        element_map.add(&input_dungeon, output_dungeon);
    }
    else {
      output_dungeon = new overworld::Dungeon(input_dungeon.get_name(), output_scope.get_overworld_scope(),
                                              input_dungeon.get_source_point());
      auto output_dungeon_owner = std::unique_ptr<overworld::Dungeon>(output_dungeon);

      element_map.add(&input_dungeon, output_dungeon);
      output_scope.get_overworld_scope().add_dungeon(std::move(output_dungeon_owner));
    }

    for (auto &generic_parameter: input_dungeon.get_generic_parameters()) {
      auto output_generic_parameter = new overworld::Generic_Parameter(
        generic_parameter.get_name(), profession_library.get_unknown(),
        overworld::Parent(*output_dungeon), generic_parameter.get_source_range()
      );
      output_dungeon->add_generic_parameter(std::unique_ptr<overworld::Generic_Parameter>(output_generic_parameter));
    }
    reflect_enchantments(input_dungeon.get_enchantments(), output_dungeon->get_enchantments(), output_scope);

    auto &enchantment_library = profession_library.get_enchantment_library();
    process_external(*output_dungeon, enchantment_library, header_files);

    Scope block_scope(output_dungeon->get_scope(), output_scope);

    reflect_scope1(input_dungeon.get_scope(), block_scope);
  }

  void Mirror::reflect_scope1(const underworld::Scope &input_scope, Scope &output_scope) {

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        reflect_function1(*input_member.second, output_scope);
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::dungeon) {
//        auto input_profession = cast<const underworld::Profession_Member>(*input_member.second)
//          .get_profession();

//        if (input_profession->get_type() == underworld::Profession_Type::dungeon) {
        auto input_dungeon = dynamic_cast<underworld::Dungeon *>(input_member.second.get());
        reflect_dungeon1(*input_dungeon, output_scope);
//        }
      }
    }
  }

  void Mirror::reflect_scope2(const underworld::Scope &input_scope, Scope &output_scope) {

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::minion) {
        auto &input_variable = *(dynamic_cast<const underworld::Minion *>(input_member.second.get()));
        reflect_minion(input_variable, output_scope);
      }
    }

//    if (output_scope.get_overworld_scope().get_scope_type() == overworld::Scope_Type::dungeon) {
//      auto &input_dungeon = cast<underworld::Dungeon>(input_scope);
//      auto &output_dungeon = output_scope.get_overworld_scope().get_parent().get_dungeon().get_original();
//      for (auto &contract:input_dungeon.get_contracts()) {
//        auto profession = reflect_profession(*contract, *output_scope.get_parent());
//        auto dungeon = get_dungeon(*profession);
////        throw std::runtime_error("Not implemented.");
//        output_dungeon.add_contract(dungeon->get_original(), profession);
//      }
//    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        reflect_function2(cast<underworld::Function>(*input_member.second), output_scope);
      }
      else if (input_member.second->get_type() == underworld::Member::Type::minion) {
        auto &input_variable = *(dynamic_cast<const underworld::Minion *>(input_member.second.get()));
        auto reflection = reflect_profession(input_variable.get_profession(), output_scope);
        auto &output_minion = *element_map.find_or_null<overworld::Minion>(&input_variable);
        profession_library.assign(output_minion.get_node(), reflection.profession,
                                  overworld::Empty_Profession_Setter::get_instance());
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::dungeon) {
//        auto input_profession = cast<const underworld::Profession_Member>(*input_member.second)
//          .get_profession();

//        if (input_profession->get_type() == underworld::Profession_Type::dungeon) {
        auto &input_dungeon = cast<underworld::Dungeon>(*input_member.second);
        auto dungeon = element_map.find_or_null<overworld::Dungeon>(&input_dungeon);
        if (!dungeon)
          throw Code_Error("Could not find overworld dungeon " + input_dungeon.get_name(),
                           input_dungeon.get_source_point());

        auto child_scope = Scope(dungeon->get_scope(), output_scope);
        reflect_scope2(input_dungeon.get_scope(), child_scope);
//        }
      }
    }
  }

  void Mirror::reflect_scope3(const underworld::Scope &input_scope, Scope &output_scope) {
    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::function) {
        reflect_function3(cast<underworld::Function_With_Block>(*input_member.second), output_scope);
      }
    }

    for (auto &input_member : input_scope.get_members()) {
      if (input_member.second->get_type() == underworld::Member::Type::dungeon) {
//        auto input_profession = cast<const underworld::Profession_Member>(*input_member.second)
//          .get_profession();

//        if (input_profession->get_type() == underworld::Profession_Type::dungeon) {
        auto &input_dungeon = cast<underworld::Dungeon>(*input_member.second);
        auto &dungeon = *element_map.find_or_null<overworld::Dungeon>(&input_dungeon);
        auto child_scope = Scope(dungeon.get_scope(), output_scope);
        reflect_scope3(input_dungeon.get_scope(), child_scope);
//        }
      }
    }
  }

  void Mirror::reflect_root(const underworld::Dungeon &input, Scope &scope) {
    reflect_scope1(input.get_scope(), scope);
    reflect_scope2(input.get_scope(), scope);
    reflect_scope3(input.get_scope(), scope);
  }
}