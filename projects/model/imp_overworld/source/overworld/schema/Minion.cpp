#include <overworld/expressions/Chain.h>
#include <overworld/expressions/Member_Expression.h>
#include "Minion.h"

namespace overworld {

  overworld::Minion *find_member_container(overworld::Expression &expression) {
    auto &chain = dynamic_cast<overworld::Chain &>(expression);
    auto &member_expression = dynamic_cast<overworld::Member_Expression &>( chain.get_first());
    auto &member = member_expression.get_member();
    if (member.get_type() != Member_Type::minion)
      return nullptr;

    return &member.get_minion();
  }

}

