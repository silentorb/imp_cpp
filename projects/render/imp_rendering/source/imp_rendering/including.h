#pragma once

#include <overworld/schema/Dungeon.h>
#include <overworld/utility/Profession_File_Map.h>
#include <overworld/exploring/Expression_Explorer.h>
#include <cpp_stl/Standard_Library.h>

namespace imp_rendering {

//  struct Profession_Reference {
//    Reference_Type type;
//      const overworld::Profession *profession;
//  };

  class Include_Manager {
      const overworld::Dungeon &dungeon;
      std::vector<overworld::File_Reference> header_includes;
      std::vector<overworld::File_Reference> source_includes;
      overworld::Profession_File_Map &header_file_map;
      std::vector<const overworld::Profession *> forward_declarations;

      void gather_include_headers();
      void gather_source_headers();

      void process_expression(const overworld::Expression &expression);

  public:
      Include_Manager(const overworld::Dungeon &dungeon, overworld::Profession_File_Map &header_file_map) :
        dungeon(dungeon), header_file_map(header_file_map) {}

//      void prepare(std::vector<const overworld::Profession *> dependencies, overworld::Profession_File_Map & file_map);

      const std::vector<overworld::File_Reference> &get_header_includes() const {
        return header_includes;
      }

      const std::vector<overworld::File_Reference> &get_source_includes() const {
        return source_includes;
      }

      void gather_headers(cpp_stl::Standard_Library &standard_library);

      const std::vector<const overworld::Profession *> &get_forward_declarations() const {
        return forward_declarations;
      }
  };
}