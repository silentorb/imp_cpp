#pragma once

#include <underworld/schema/Dungeon.h>
#include <overworld/schema/Dungeon.h>
#include <underworld/schema/Profession_Library.h>
#include <overworld/schema/professions/Profession_Library.h>
#include <imp_summoning/Lookup.h>
#include <imp_mirror/Element_Map.h>
#include <source_mapping/Source_Point.h>
#include <overworld/imp_graph/Graph.h>
#include <imp_mirror/Temporary_Interface_Manager.h>
#include <cpp_stl/Standard_Library.h>

namespace imp_wrapper {

  class Wrapper_Internal {
      underworld::Dungeon underworld_root;
      overworld::Dungeon overworld_root;
      runic::Lookup runic_lookup;
      const runic_imp::Symbols lexicon;
      imp_summoning::Lookup lookup;
      overworld::Graph graph;
      overworld::Profession_Library overworld_profession_library;
      imp_mirror::Element_Map element_map;
      std::vector<std::unique_ptr<source_mapping::Source_File>> source_files;
      cpp_stl::Standard_Library *standard_library;

  public:
      Wrapper_Internal();
      virtual ~Wrapper_Internal();
      void load_file(const std::string &path);
      void mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager);
      void solve();
      void render(const std::string &output_path);
  };
}