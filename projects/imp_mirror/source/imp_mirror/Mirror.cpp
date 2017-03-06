#include "Mirror.h"

namespace imp_mirror {

  void Mirror::reflect(const underworld::Dungeon &input, overworld::Dungeon &output) {
    for (auto &entry: input.get_dungeons()) {
      auto output_dungeon = overworld::Dungeon_Owner(new overworld::Dungeon(*entry.second));
      output.add_dungeon(output_dungeon);
    }
  }
}