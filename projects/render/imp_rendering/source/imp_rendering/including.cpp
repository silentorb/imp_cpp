#include "including.h"

using namespace overworld;

namespace imp_rendering {

  bool has_file(std::vector<File *> &files, File &file) {
    for (auto item: files) {
      if (item == &file)
        return true;;
    }

    return false;
  }

  void Include_Manager::prepare(std::vector<const Profession *> dependencies, Profession_File_Map &file_map) {
    for (auto dependency: dependencies) {
      auto &profession = *dependency;

      auto file = profession.get_file();
      if (!file)
        file = file_map.get_file(profession);

      if (file) {
        if (!has_file(source_includes, *file)) {
          profession.get_file();
          source_includes.push_back(file);
        }
      }
    }

    source_includes.push_back(file_map.get_file(dungeon));
  }
}