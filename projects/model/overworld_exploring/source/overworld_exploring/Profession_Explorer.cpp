#include <overworld/schema/Function.h>
#include <overworld/expressions/Assignment.h>
#include "Profession_Explorer.h"

namespace overworld {
  namespace exploring {

    void Profession_Explorer::explore_expression(const Expression &expression) {
      switch (expression.get_type()) {

        case Expression::Type::assignment: {
          auto &assignment = *dynamic_cast<const Assignment *>(&expression);
          explore_expression(*assignment.get_target());
          explore_expression(*assignment.get_value());
          break;
        }

      }
    }

    void Profession_Explorer::explore_block(const Block &block) {
      explore_scope(block.get_scope());

      for (auto &expression : block.get_expressions()) {
        explore_expression(*expression);
      }
    }

    void Profession_Explorer::explore_scope(const Scope &scope) {
      for (auto &minion: scope.get_minions()) {
        explore_profession(minion->get_profession());
      }

      for (auto &function: scope.get_functions()) {
        explore_function(*function);
      }
    }

    void Profession_Explorer::explore_profession(const Profession &profession) {
      action(profession);
    }

    void Profession_Explorer::explore_function(const Function &function) {
      explore_profession(function.get_profession());
      explore_block(function.get_block());
    }

    void Profession_Explorer::explore(const overworld::Dungeon &dungeon) {
      explore_scope(dungeon);

      for (auto &child: dungeon.get_dungeons()) {
        action(*child);
        explore(*child);
      }
    }

  }
}