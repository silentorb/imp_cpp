#include <overworld/expressions/Lambda.h>
#include <overworld/schema/Temporary_Minion.h>
#include <overworld/expressions/Chain.h>
#include "Connector.h"

using namespace overworld;

namespace imp_mirror {

  overworld::Function_Signature &Connector::get_function_signature(overworld::Expression &expression,
                                                                   std::vector<overworld::Expression_Owner> &arguments,
                                                                   Scope &scope) {
    if (expression.get_type() == overworld::Expression_Type::member) {
      auto member_expression = dynamic_cast<overworld::Member_Expression *>(&expression);
      auto &member = member_expression->get_member();
      if (member.get_type() == overworld::Member_Type::function) {
        return member.get_function().get_signature();
      }
      else if (member.get_type() == overworld::Member_Type::dungeon) {
        auto &dungeon = member.get_dungeon();
        return dungeon.get_or_create_constructor().get_signature();
      }
      else if (member.get_type() == overworld::Member_Type::minion) {
//        auto &temporary_member = dynamic_cast<overworld::Temporary_Minion &>(member.get_minion());
//        auto &signature = temporary_member.get_or_create_signature();
//        for (auto i = signature.get_parameters().size(); i < arguments.size(); ++i) {
//          auto &argument = arguments[i];
//          auto &argument_profession = argument->get_node()->get_profession();
//          auto parameter = new overworld::Parameter("(temp)", argument_profession,
//                                                    source_mapping::Source_Range(),
//                                                    *scope.get_overworld_scope().get_function());
//          temporary_member.add_parameter(overworld::Parameter_Owner(parameter));
//        }
//        return signature;
      }
      else {
        throw std::runtime_error("Expression is not a function.");
      }

    }

    throw std::runtime_error("Expression is not a function.");
  }

  overworld::Minion &find_member_container(overworld::Expression &expression) {
    auto &chain = dynamic_cast<overworld::Chain &>(expression);
    auto &member_expression = dynamic_cast<overworld::Member_Expression &>( chain.get_first());
    auto &minion = member_expression.get_member().get_minion();
    return minion;
  }

  void Connector::connect_parameters_and_arguments(overworld::Invoke &invoke,
                                                   const std::vector<underworld::Expression_Owner> &source_arguments,
                                                   Scope &scope) {
    auto &overworld_function = get_function_signature(invoke.get_expression().get_last(),
                                                      invoke.get_arguments(), scope);
    auto &parameters = overworld_function.get_elements();
    auto &invoke_arguments = invoke.get_arguments();
    if (parameters.size() - 1 != invoke_arguments.size())
      throw std::runtime_error("Parameter and argument lengths do not match.");

    for (int i = 0; i < invoke_arguments.size(); ++i) {
      auto &parameter = *parameters[i];
      auto &first = parameter.get_node();
      auto &argument = *invoke_arguments[i];
      auto &second = *argument.get_node();
      auto k = first.get_element().get_type();
      if (&first.get_parent().get_function() != &second.get_parent().get_function()
          && first.get_profession().get_type() == Profession_Type::generic_parameter) {
        auto &member_container = find_member_container(invoke.get_expression());
        auto &source_argument = *source_arguments[i];
//        auto argument_node = new Argument_Node(first.get_profession(),
//                                               member_container,
//                                               scope.get_overworld_scope().get_function(),
//                                               profession_library,
//                                               source_argument.get_source_point());
//        invoke.add_argument_node(std::unique_ptr<Argument_Node>(argument_node));
//        throw std::runtime_error("Not implemented.");
//        auto connection = new Component_To_Scalar(*argument_node, second);
//        graph.connect(*argument_node, second, std::unique_ptr<Connection>(connection));
      }
      else {
        graph.connect(first, second);
        if (argument.get_type() == Expression_Type::lambda) {
          auto &lambda = static_cast<Lambda &>(argument);
          auto &member_container = find_member_container(invoke.get_expression());
//          auto &parameter_profession = parameter.get_profession().get_base();
//          if (parameter_profession.get_type() == Profession_Type::function) {
//
////            graph.connect();
//          }
//          else {
//            throw std::runtime_error("Not supported.");
//          }
        }
      }
    }
  }
}