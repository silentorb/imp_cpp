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
      new_member->add_expression(*result);
      return Expression_Owner(result);
    }

    throw std::runtime_error("Not implemented.");
  }

  void Temporary_Interface::replace(Profession_Reference &profession) {
    if (profession.get_type() == Profession_Type::dungeon) {
      auto &dungeon = profession->get_dungeon_interface().get_original();
      auto &scope = dungeon.get_scope();
      for (auto &member : members) {
        auto new_member = scope.get_member_or_null(member->get_name());
        if (!new_member)
          throw std::runtime_error(dungeon.get_name() + " does not have a member named " + member->get_name());

        member->replace_with(*new_member);
      }
    }
    else {
      throw std::runtime_error("Not supported.");
    }
  }
}