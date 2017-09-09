
#include "Wrapper.h"
#include "Wrapper_Internal.h"

namespace imp_wrapper {

  Wrapper::Wrapper() :
    internal(new Wrapper_Internal()) {
  }

  Wrapper::~Wrapper() {

  }

  void Wrapper::load_file(const std::string &path) {
    internal->load_file(path);
  }

  void Wrapper::mirror(imp_mirror::Temporary_Interface_Manager &temporary_interface_manager) {
    internal->mirror(temporary_interface_manager);
  }

  void Wrapper::solve() {
    internal->solve();
  }

  void Wrapper::render(const std::string &output_path) {
    internal->render(output_path);
  }

}