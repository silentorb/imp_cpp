#include <overworld/expressions/Chain.h>
#include <overworld/expressions/Member_Expression.h>
#include "Minion.h"

namespace overworld {

//  Dungeon *Minion::get_dungeon() {
//    auto &dungeon = element.get_parent().get_dungeon();
//    return dynamic_cast<Dungeon *>(&dungeon);
//  }

//  const std::string Minion_Element::get_name() const {
//    return std::string();
//  }

  overworld::Minion &find_member_container(overworld::Expression &expression) {
    auto &chain = dynamic_cast<overworld::Chain &>(expression);
    auto &member_expression = dynamic_cast<overworld::Member_Expression &>( chain.get_first());
    auto &minion = member_expression.get_member().get_minion();
    return minion;
  }

}

