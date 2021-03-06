#include <overworld/schema/Function.h>
#include <overworld/expressions/Assignment.h>
#include <overworld/expressions/Minion_Declaration.h>
#include <overworld/expressions/Invoke.h>
#include <overworld/expressions/Lambda.h>
#include <overworld/expressions/Chain.h>
#include "Expression_Explorer.h"

namespace overworld {
  namespace exploring {

    void Expression_Explorer::explore_expression(const Expression &expression) {
      action(expression);

      switch (expression.get_type()) {

        case Expression_Type::assignment: {
          auto &assignment = *static_cast<const Assignment *>(&expression);
          explore_expression(assignment.get_target());
          explore_expression(assignment.get_value());
          break;
        }

        case Expression_Type::variable_declaration_and_assignment: {
          auto &assignment = *static_cast<const Minion_Declaration_And_Assignment *>(&expression);
          explore_expression(assignment.get_expression());
          break;
        }

        case Expression_Type::invoke: {
          auto &invoke = static_cast<const Invoke &>(expression);
          for (auto &argument:invoke.get_arguments()) {
            explore_expression(*argument);
          }
          explore_expression(invoke.get_expression());
          break;
        }

        case Expression_Type::lambda: {
          auto &lambda = static_cast<const Lambda &>(expression);
          explore_block(lambda.get_function().get_block());
          break;
        }

        case Expression_Type::chain: {
          auto &chain = static_cast<const Chain &>(expression);
          for (auto &link: chain.get_expressions()) {
            explore_expression(*link);
          }
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
      auto with_block = dynamic_cast<const Function_With_Block *>(&function);
      if (with_block)
        explore_block(with_block->get_block());
    }

//    void Expression_Explorer::explore(const overworld::Dungeon &dungeon) {
//      explore_scope(dungeon);
//    }

  }
}