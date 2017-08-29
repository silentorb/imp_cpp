#include <overworld/schema/Function.h>
#include <overworld/expressions/Assignment.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/expressions/Invoke.h>
#include "Expression_Explorer.h"

namespace overworld {
  namespace exploring {

    void Expression_Explorer::explore_expression(const Expression &expression) {
      action(expression);

      switch (expression.get_type()) {

        case Expression::Type::assignment: {
          auto &assignment = *dynamic_cast<const Assignment *>(&expression);
          explore_expression(*assignment.get_target());
          explore_expression(*assignment.get_value());
          break;
        }

        case Expression::Type::variable_declaration_and_assignment: {
          auto &assignment = *dynamic_cast<const Minion_Declaration_And_Assignment *>(&expression);
          explore_expression(assignment.get_expression());
          break;
        }

        case Expression::Type::invoke: {
          auto &invoke = dynamic_cast<const Invoke &>(expression);
          for (auto &argument:invoke.get_arguments()) {
            explore_expression(*argument);
          }
          break;
        }

      }
    }

    void Expression_Explorer::explore_block(const Block &block) {
      explore_scope(block.get_scope());

      for (auto &expression : block.get_expressions()) {
        explore_expression(*expression);
      }
    }

    void Expression_Explorer::explore_scope(const Scope &scope) {
      for (auto &function: scope.get_functions()) {
        explore_function(*function);
      }
    }

    void Expression_Explorer::explore_function(const Function &function) {
      explore_block(function.get_block());
    }

//    void Expression_Explorer::explore(const overworld::Dungeon &dungeon) {
//      explore_scope(dungeon);
//    }

  }
}