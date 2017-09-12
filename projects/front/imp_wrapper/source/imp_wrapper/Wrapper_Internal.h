#pragma once

#include <underworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon.h>
#include <overworld/schema/professions/Profession_Library.h>
#include <imp_summoning/Parser_Lookup.h>
#include <imp_mirror/Element_Map.h>
#include <source_mapping/Source_Point.h>
#include <overworld/imp_graph/Graph.h>
#include <imp_mirror/Temporary_Interface_Manager.h>
#include <cpp_stl/Standard_Library.h>
#include <imp_summoning/Zookeeper.h>

namespace imp_wrapper {

  class Wrapper_Internal {
      underworld::Dungeon underworld_root;
      overworld::Dungeon overworld_root;
      overworld::Graph graph;
      overworld::Profession_Library overworld_profession_library;
      imp_mirror::Element_Map element_map;
      cpp_stl::Standard_Library *standard_library;
      imp_summoning::Zookeeper zookeeper;
  public:
      Wrapper_Internal();
      virtual ~Wrapper_Internal();
      void load_file(const std::string &path);
      void mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager);
      void solve();
      void render(const std::string &output_path);
  };
}