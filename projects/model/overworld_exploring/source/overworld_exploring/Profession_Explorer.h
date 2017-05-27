#pragma once

#include <overworld/schema/Dungeon.h>
#include <functional>
#include <overworld/expressions/Block.h>

namespace overworld {
  namespace exploring {

    using Profession_Delegate = std::function<void(const Profession &)>;

    class Profession_Explorer {
        const Profession_Delegate action;

        void explore_expression(const Expression &expression);
        void explore_profession(const Profession &profession);
        void explore_function(const Function &function);
        void explore_block(const Block &block);
        void explore_scope(const Scope &scope);

    public:
        Profession_Explorer(const Profession_Delegate &action) : action(action) {}

        void explore(const Dungeon &dungeon);
    };
  }
}