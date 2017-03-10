#include "sources.h"
#include <overworld/schema/Function.h>

using namespace imp_artisan;
using namespace std;

namespace imp_rendering {
  namespace sources {

    imp_artisan::Stroke *render_function(const overworld::Function &function) {
      auto block = new imp_artisan::Block("void " + function.get_name(), "}");
      for(auto &statement : function.get_block().get_expressions()) {

      }

      return block;
    }

    void render(const overworld::Dungeon &dungeon, Stream &strokes) {
    }
  }
}