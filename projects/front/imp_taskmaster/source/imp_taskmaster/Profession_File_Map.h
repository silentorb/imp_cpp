#pragma once

#include <overworld/schema/Dungeon.h>
#include <unordered_map>
#include <overworld_exploring/Profession_File_Map.h>

namespace imp_taskmaster {

  class Profession_File_Map : public overworld::exploring::Profession_File_Map {
      std::unordered_map<const overworld::Profession *, std::unique_ptr<overworld::File>> files;

  public:
      void set(overworld::Profession &dungeon, std::unique_ptr<overworld::File> file) {
        files.insert(std::make_pair(&dungeon, std::move(file)));
      }

      overworld::File *get_file(const overworld::Profession &dungeon) override {
        return files.count(&dungeon) > 0
               ? files[&dungeon].get()
               : nullptr;
      }
  };
}