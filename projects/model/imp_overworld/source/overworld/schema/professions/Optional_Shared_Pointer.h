#pragma once

#include <memory>

namespace overworld {

  template<typename T>
  class Optional_Shared_Pointer {
      T *pointer = nullptr;
      std::shared_ptr<T> shared_pointer;

  public:
//      Optional_Shared_Pointer() {}

      explicit Optional_Shared_Pointer(T &pointer) : pointer(&pointer) {}

//      explicit Optional_Shared_Pointer(std::shared_ptr<T> &shared_pointer) : shared_pointer(shared_pointer) {}
      explicit Optional_Shared_Pointer(std::shared_ptr<T> shared_pointer) : shared_pointer(shared_pointer) {}

      T &operator*() const {
        return pointer
               ? *pointer
               : *shared_pointer;
      }

      T *get() const {
        return pointer
               ? pointer
               : shared_pointer.get();
      }

      T *operator->() {
        return pointer
               ? pointer
               : shared_pointer.get();
      }

      const T *operator->() const {
        return pointer
               ? pointer
               : shared_pointer.get();
      }
  };
}