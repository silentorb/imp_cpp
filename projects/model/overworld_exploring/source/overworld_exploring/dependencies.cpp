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

    void gather_header_dependencies(std::vector<File *> &files, const Dungeon &dungeon) {
      Profession_Explorer explorer([& files](const Profession &profession) {
        auto file = profession.get_file();
        if (file) {
          if (!has_file(files, *file)) {
            files.push_back(file);
          }
        }
      });
      explorer.explore(dungeon);
    }
  }
}