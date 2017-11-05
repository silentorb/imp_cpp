#include "Temporary_Interface.h"
#include "Function.h"
#include <underworld/expressions/Member_Expression.h>

namespace overworld {

  Expression_Owner Temporary_Interface::create_temporary_member(Member_Expression &previous_expression,
                                                                const std::string &child_name,
                                                                const source_mapping::Source_Range source_point,
                                                                Graph &graph,
                                                                Scope &scope) {
    auto &previous_member = previous_expression.get_member();
    if (previous_member.get_type() == Member_Type::parameter) {
//      auto &parameter = previous_member.get_parameter();
//      auto function = static_cast<Function_With_Block *>(&parameter.get_node().get_parent().get_function());
      auto new_member = new Temporary_Member(child_name,
                                             Profession_Library::get_unknown(),
                                             source_point,
                                             scope.get_owner().get_function());
      members.push_back(std::unique_ptr<Temporary_Member>(new_member));

      auto result = new Member_Expression(*new_member, source_point);
      return Expression_Owner(result);
    }

    throw std::runtime_error("Not implemented.");
  }

}