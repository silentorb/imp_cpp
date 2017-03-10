#include "sources.h"
#include <overworld/schema/Function.h>
#include <imp_artisan/building.h>

using namespace imp_artisan::building;
using namespace std;

namespace imp_rendering {
  namespace sources {

    Stroke render_statement(const overworld::Expression &expression) {
      return string();
    }

    Stroke render_includes(const overworld::Dungeon &dungeon) {
      Stroke result;
      result << "#include \"" + dungeon.get_name() + ".h\"";
      return result;
    }

    Stroke render_function(const overworld::Function &function, const overworld::Dungeon &dungeon) {
      auto function_signature = render_function_return_signature(function)
                                + dungeon.get_name() + "::"
                                + function.get_name()
                                + render_function_parameters(function);
      Stroke block(new imp_artisan::internal::Block(function_signature, "}"));
      for (auto &statement : function.get_block().get_expressions()) {
        block << render_statement(*statement);
      }

      return move(block);
    }

    Stroke render_function(const overworld::Function &function) {
      auto function_signature = render_function_declaration(function);
      auto block = Block(function_signature, "}");
      for (auto &statement : function.get_block().get_expressions()) {

      }

      return move(block);
    }

    Stroke render(const overworld::Dungeon &dungeon) {
      Stroke result;
      result << render_includes(dungeon);
      for (auto &function : dungeon.get_functions()) {
        result << render_function(*function, dungeon);
      }

      return result;
    }
  }
}