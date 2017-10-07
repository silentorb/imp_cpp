#pragma once

#include <memory>
#include <imp_mirror/Temporary_Interface_Manager.h>

namespace underworld {
  class Dungeon;
}

namespace imp_wrapper {

  class Wrapper_Internal;

  class Wrapper {
      std::unique_ptr<Wrapper_Internal> internal;

  public:
      Wrapper();
      ~Wrapper();

      void load_file(const std::string &path, underworld::Dungeon &underworld_root);
      void mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager, underworld::Dungeon &underworld_root);
      void solve();
      void render(const std::string &output_path);
  };
}