#include "Mirror.h"

namespace imp_mirror {

  const overworld::Profession &Mirror::convert_profession(const underworld::Profession &profession) {
    return profession_library.get_unknown();
  }

  void Mirror::reflect_dungeon(const underworld::Dungeon &input, overworld::Dungeon &output) {
    for (auto &entry: input.get_members()) {
      auto &member = *entry.second;
      if (member.get_type() == underworld::Member::Type::minion) {
        auto &input_minion = dynamic_cast<underworld::Minion &>(member);
        auto &profession = convert_profession(input_minion.get_profession());
        auto &output_minion = output.create_minion(input_minion, profession);
      }
      else {
        auto &input_function = dynamic_cast<underworld::Function &>(member);
        auto &output_function = output.create_function(input_function);

      }
    }

    for (auto &entry: input.get_dungeons()) {
      auto &input_dungeon = *entry.second;
      auto output_dungeon = overworld::Dungeon_Owner(new overworld::Dungeon(input_dungeon));
      output.add_dungeon(output_dungeon);
      reflect_dungeon(input_dungeon, *output_dungeon);
    }
  }
}