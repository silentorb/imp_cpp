#include <overworld/schema/Function.h>
#include "headers.h"

using namespace imp_artisan;
using namespace overworld;

using namespace std;

namespace imp_rendering {
  namespace headers {

    imp_artisan::Stroke *render_function(const overworld::Function &function) {
      return new Text("void " + function.get_name() + "()" + ";");
    }

    Stroke *render_dungeon_body(const overworld::Dungeon &dungeon) {
      auto block = new imp_artisan::Block("class " + dungeon.get_name(), "};");
      *block << new Special_Text("public:");

      for (auto &entry: dungeon.get_members()) {
        auto &member = *entry.second;
        if (member.get_type() == overworld::Member::Type::variable) {
          auto &dungeon_minion = *dynamic_cast<overworld::Minion *>(&member);
//          auto &profession = reflect_profession(dungeon_minion.get_profession());
//          auto &output_minion = output.create_minion(dungeon_minion, profession);
        }
        else {
          auto &function = *dynamic_cast<overworld::Function *>(&member);
          *block << render_function(function);
        }
      }

      for (auto &entry: dungeon.get_dungeons()) {
//        auto &dungeon_dungeon = *entry.second;
//        auto &output_dungeon = output.create_dungeon(dungeon_dungeon);
//        reflect_dungeon(dungeon_dungeon, output_dungeon);
      }
      return block;
    }

    void render(const Dungeon &dungeon, Stream &strokes) {
      strokes << new Group({new Text("#pragma once")})
              << render_dungeon_body(dungeon);
    }
  }
}