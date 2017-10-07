
#include "Wrapper.h"
#include "Wrapper_Internal.h"

namespace imp_wrapper {

  Wrapper::Wrapper() :
    internal(new Wrapper_Internal()) {
  }

  Wrapper::~Wrapper() {

  }

  void Wrapper::load_file(const std::string &path, underworld::Dungeon &underworld_root) {
    internal->load_file(path, underworld_root);
  }

  void Wrapper::mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager, underworld::Dungeon &underworld_root) {
    internal->mirror(temporary_interface_manager, underworld_root);
  }

  void Wrapper::solve() {
    internal->solve();
  }

  void Wrapper::render(const std::string &output_path) {
    internal->render(output_path);
  }

}

// TODO iterators
// TODO while loops
// TODO ranges
// TODO enums
// TODO casting