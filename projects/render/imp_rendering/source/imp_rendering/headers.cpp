#include "headers.h"
#include <overworld/schema/Function.h>

using namespace imp_artisan;
using namespace overworld;

using namespace std;

namespace imp_rendering {
  namespace headers {

    imp_artisan::Stroke *render_function(const overworld::Function &function) {
      return new Text(render_function_declaration(function) + ";");
    }

    Stroke *render_dungeon_body(const overworld::Dungeon &dungeon) {
      auto block = new imp_artisan::Block("class " + dungeon.get_name(), "};", 2);
      *block << new Special_Text("public:");

      for (auto &variable : dungeon.get_variables()) {
        //          auto &profession = reflect_profession(dungeon_minion.get_profession());
//          auto &output_minion = output.create_minion(dungeon_minion, profession);

      }

      for (auto &function : dungeon.get_functions()) {
        *block << render_function(*function);
      }
      return block;
    }

    void render(const Dungeon &dungeon, Stream &strokes) {
      strokes << new Group({new Text("#pragma once")})
              << render_dungeon_body(dungeon);
    }
  }
}