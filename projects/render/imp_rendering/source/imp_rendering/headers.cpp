#include "headers.h"
#include <overworld/schema/Function.h>
#include <imp_artisan/building.h>

using namespace imp_artisan::building;
using namespace overworld;

using namespace std;

namespace imp_rendering {
  namespace headers {

    Stroke render_function(const overworld::Function &function) {
      return render_function_declaration(function) + ";";
    }

    Stroke render_dungeon_body(const overworld::Dungeon &dungeon) {
      Stroke block(new imp_artisan::internal::Block("class " + dungeon.get_name(), "};", 2));
      block << Stroke(new imp_artisan::internal::Special_Text("public:"));

      for (auto &variable : dungeon.get_variables()) {
        //          auto &profession = reflect_profession(dungeon_minion.get_profession());
//          auto &output_minion = output.create_minion(dungeon_minion, profession);

      }

      for (auto &function : dungeon.get_functions()) {
        block << render_function(*function);
      }
      return block;
    }

    Stroke render(const Dungeon &dungeon) {
//      return Stroke()
//        << (Stroke() << "#pragma once")
//                     << render_dungeon_body(dungeon);
      Stroke result;
      result << wrap("#pragma once")
             << render_dungeon_body(dungeon);
      return result;
    }
  }
}