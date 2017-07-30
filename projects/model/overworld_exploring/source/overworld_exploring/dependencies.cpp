#include "dependencies.h"
#include "overworld/exploring/Profession_Explorer.h"

namespace overworld {
  namespace exploring {

/*
    void gather_header_dependencies(std::vector<File *> &files, const Dungeon &dungeon, Profession_File_Map &file_map) {
      Profession_Explorer explorer([&files, &file_map, &dungeon](const Profession &profession) {
        if (&profession == &dungeon)
          return;

        auto file = profession.get_file();
        if (!file)
          file = file_map.get_file(profession);

        if (file) {
          if (!has_file(files, *file)) {
            profession.get_file();
            files.push_back(file);
          }
        }
      });
      explorer.explore(dungeon);
    }
*/

//    void gather_dungeon_dependencies(std::vector<const Profession *> &dependencies, const Dungeon &dungeon) {
//      Profession_Explorer explorer([&dungeon, &dependencies](const Profession &profession) {
//        if (&profession == &dungeon)
//          return;
//
//        dependencies.push_back(&profession);
//      });
//      explorer.explore_block(dungeon);
//    }
  }
}