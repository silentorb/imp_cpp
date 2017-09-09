#pragma once

#include <memory>
#include <imp_mirror/Temporary_Interface_Manager.h>

namespace imp_wrapper {

  class Wrapper_Internal;

  class Wrapper {
      std::unique_ptr<Wrapper_Internal> internal;

  public:
      Wrapper();
      ~Wrapper();

      void load_file(const std::string &path);
      void mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager);
      void solve();
      void render(const std::string &output_path);
  };
}