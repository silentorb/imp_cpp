#pragma once

#include <overworld/schema/Dungeon.h>
#include <overworld/utility/Profession_File_Map.h>

namespace imp_rendering {

  class Include_Manager {
      const overworld:: Dungeon & dungeon;
      std::vector<overworld::File *> header_includes;
      std::vector<overworld::File *> source_includes;

  public:
      Include_Manager(const overworld::Dungeon &dungeon) : dungeon(dungeon) {}

      void prepare(std::vector<const overworld::Profession *> dependencies, overworld::Profession_File_Map & file_map);

      const std::vector<overworld::File *> &get_header_includes() const {
        return header_includes;
      }

      const std::vector<overworld::File *> &get_source_includes() const {
        return source_includes;
      }
  };
}