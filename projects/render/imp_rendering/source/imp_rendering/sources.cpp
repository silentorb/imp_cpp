#include "sources.h"
#include <overworld/schema/Function.h>

using namespace imp_artisan;
using namespace std;

namespace imp_rendering {
  namespace sources {

    imp_artisan::Stroke *render_statement(const overworld::Expression &expression) {

    }

    imp_artisan::Stroke *render_includes(const overworld::Dungeon &dungeon) {
      auto result = new imp_artisan::Group();
      *result << "#include \"" + dungeon.get_name() + ".h\"";
      return result;
    }

    imp_artisan::Stroke *render_function(const overworld::Function &function, const overworld::Dungeon &dungeon) {
      auto function_signature = render_function_return_signature(function)
                                + dungeon.get_name() + "::"
                                + function.get_name()
                                + render_function_parameters(function);
      auto block = new imp_artisan::Block(function_signature, "}");
      for (auto &statement : function.get_block().get_expressions()) {
        *block << render_statement(*statement);
      }

      return block;
    }

    imp_artisan::Stroke *render_function(const overworld::Function &function) {
      auto function_signature = render_function_declaration(function);
      auto block = new imp_artisan::Block(function_signature, "}");
      for (auto &statement : function.get_block().get_expressions()) {

      }

      return block;
    }

    void render(const overworld::Dungeon &dungeon, Stream &strokes) {
      strokes << render_includes(dungeon);
      for (auto &function : dungeon.get_functions()) {
        strokes << render_function(*function, dungeon);
      }
    }
  }
}