#include <overworld/expressions/Lambda.h>
#include <overworld/schema/Temporary_Interface.h>
#include <overworld/expressions/Chain.h>
#include "Connector.h"

using namespace overworld;

namespace imp_mirror {
//
//  overworld::Function_Signature &Connector::get_function_signature(overworld::Expression &expression,
//                                                                   std::vector<overworld::Expression_Owner> &arguments,
//                                                                   Scope &scope) {
//    if (expression.get_type() == overworld::Expression_Type::member) {
//      auto member_expression = dynamic_cast<overworld::Member_Expression *>(&expression);
//      auto &member = member_expression->get_member();
//      if (member.get_type() == overworld::Member_Type::function) {
//        return member.get_function().get_signature();
//      }
//      else if (member.get_type() == overworld::Member_Type::dungeon) {
//        auto &dungeon = member.get_dungeon();
//        return dungeon.get_or_create_constructor().get_signature();
//      }
//      else if (member.get_type() == overworld::Member_Type::minion) {
////        auto &temporary_member = dynamic_cast<overworld::Temporary_Minion &>(member.get_minion());
////        auto &signature = temporary_member.get_or_create_signature();
////        for (auto i = signature.get_parameters().size(); i < arguments.size(); ++i) {
////          auto &argument = arguments[i];
////          auto &argument_profession = argument->get_node()->get_profession();
////          auto parameter = new overworld::Parameter("(temp)", argument_profession,
////                                                    source_mapping::Source_Range(),
////                                                    *scope.get_overworld_scope().get_function());
////          temporary_member.add_parameter(overworld::Parameter_Owner(parameter));
////        }
////        return signature;
//      }
//      else {
//        throw std::runtime_error("Expression is not a function.");
//      }
//
//    }
//
//    throw std::runtime_error("Expression is not a function.");
//  }

  void Connector::connect_parameters_and_arguments(overworld::Invoke &invoke,
                                                   const std::vector<underworld::Expression_Owner> &source_arguments,
                                                   Scope &scope) {
    auto &function = invoke.get_function(*graph);
    auto &signature = function.get_signature();
    auto &parameters = signature.get_elements();
    auto &invoke_arguments = invoke.get_arguments();
    if (parameters.size() - 1 != invoke_arguments.size())
      throw std::runtime_error("Parameter and argument lengths do not match.");

    for (int i = 0; i < invoke_arguments.size(); ++i) {
      auto &parameter = *parameters[i];
      auto &first = parameter.get_node();
      auto &argument = *invoke_arguments[i];
      auto &second = *argument.get_node();
      if (second.get_parent().get_type() == Parent_Type::function
          && &first.get_parent().get_function() != &second.get_parent().get_function()
          && parameter.get_generic_argument()) {
        auto member_container = find_member_container(invoke.get_expression());
        auto &source_argument = *source_arguments[i];
        auto connection = new Variant_To_Argument(member_container->get_node(), second, i);
        graph->connect(member_container->get_node(), second, std::unique_ptr<Connection>(connection));
      }
      else {
        auto &parent = first.get_parent();
        auto &dungeon_parent = parent.get_type() == Parent_Type::dungeon
                               ? parent.get_dungeon()
                               : parent.get_function().get_element().get_parent().get_dungeon();

        if (!dungeon_parent.is_generic()) {
          graph->connect(first, second);
        }

        if (argument.get_type() == Expression_Type::lambda) {
          auto &lambda = static_cast<Lambda &>(argument);
          auto member_container = find_member_container(invoke.get_expression());
          auto connection = new Variant_To_Lambda(member_container->get_node(), second, 0, i);
          graph->connect(member_container->get_node(), second, std::unique_ptr<Connection>(connection));
        }
      }
    }
  }

  void Connector::connect_lambda_parameters(overworld::Node &lambda_node, overworld::Function_Signature &signature) {
    if (!graph)
      return;

    for (int i = 0; i < signature.get_elements().size(); ++i) {
      auto &element = signature.get_elements()[i];
      auto connection = new Lambda_To_Parameter(lambda_node, element->get_node(), i);
      graph->connect(lambda_node, element->get_node(), std::unique_ptr<Connection>(connection));
    }
  }
}