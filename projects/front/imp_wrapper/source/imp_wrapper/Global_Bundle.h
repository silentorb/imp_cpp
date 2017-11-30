#pragma once

#include <overworld/imp_graph/Graph.h>
#include <overworld/schema/professions/Profession_Library.h>
#include <imp_summoning/Zookeeper.h>
#include <imp_mirror/Element_Map.h>
#include <imp_mirror/Scope.h>

namespace underworld {
  class Dungeon;
}

namespace lifetime {
  class Graph;
}

namespace overworld {
  class Dungeon;
}

namespace cpp_stl {
  class Standard_Library;
}

namespace imp_mirror {
  class Temporary_Interface_Manager;
}

namespace imp_wrapper {

  class Project_Bundle;

  class Global_Bundle {
      overworld::Profession_Library overworld_profession_library;
      imp_mirror::Element_Map element_map;
      cpp_stl::Standard_Library *standard_library;
      imp_summoning::Zookeeper zookeeper;
      overworld::File_Library header_files;
      std::unordered_map<std::string, overworld::Dungeon_Owner> realms;

      void initialize_standard_library();
      overworld::Dungeon &create_realm(const std::string &name);

  public:
      Global_Bundle();
      virtual ~Global_Bundle();
      void load_file(const std::string &path, underworld::Dungeon &underworld_root);
      void mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager,
                  Project_Bundle &project_bundle);
      void solve(Project_Bundle &project_bundle);
      void render(const std::string &output_path, overworld::Dungeon &overworld_root);
      std::unique_ptr<Project_Bundle> create_project_bundle(const std::string &name);
      overworld::Dungeon *get_realm(const std::string &name);
  };
}