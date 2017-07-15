#include "dependencies.h"
#include "Profession_Explorer.h"

namespace overworld {
  namespace exploring {

    bool has_file(std::vector<File *> &files, File &file) {
      for (auto item: files) {
        if (item == &file)
          return true;;
      }

      return false;
    }

    void gather_header_dependencies(std::vector<File *> &files, const Dungeon &dungeon, Profession_File_Map &file_map) {
      Profession_Explorer explorer([&files, &file_map](const Profession &profession) {
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
  }
}