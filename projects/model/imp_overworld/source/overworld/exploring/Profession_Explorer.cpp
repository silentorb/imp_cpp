#include <overworld/schema/Function.h>
#include <overworld/expressions/Assignment.h>
#include "Profession_Explorer.h"
#include "Expression_Explorer.h"

namespace overworld {
  namespace exploring {

    void Profession_Explorer::explore_expression(const Expression &expression) {
      if (expression.is_statement())
        return;

      if (expression.get_type() == Expression::Type::literal
          || expression.get_type() == Expression::Type::member
          || expression.get_type() == Expression::Type::chain)
        return;

      auto node = const_cast<Expression *>(&expression)->get_node();
      auto &profession = node->get_element().get_profession();
      if (profession.get_type() != Profession_Type::unknown && profession.get_type() != Profession_Type::Void)
        action(profession);
    }

//    void Profession_Explorer::explore_profession(const Profession &profession) {
//      action(profession);
//    }

    void Profession_Explorer::explore_block(const overworld::Block &block) {
      Expression_Explorer explorer([this](const Expression &expression) {
        explore_expression(expression);
      });

      explorer.explore_block(block);
    }

  }
}