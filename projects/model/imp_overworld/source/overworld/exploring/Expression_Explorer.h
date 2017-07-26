#pragma once

#include <overworld/schema/Dungeon.h>
#include <functional>
#include <overworld/expressions/Block_Expression.h>

namespace overworld {
  namespace exploring {

    using Expression_Delegate = std::function<void(const Expression &)>;

    class Expression_Explorer {
        const Expression_Delegate action;

        void explore_expression(const Expression &expression);
//        void explore_profession(const Profession &profession);
        void explore_function(const Function &function);
        void explore_scope(const Scope &scope);

    public:
        Expression_Explorer(const Expression_Delegate &action) : action(action) {}

        void explore_block(const Block &block);
    };
  }
}