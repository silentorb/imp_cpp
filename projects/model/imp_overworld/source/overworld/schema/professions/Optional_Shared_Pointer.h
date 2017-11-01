#pragma once

#include <memory>

namespace overworld {

  template<typename T>
  class Optional_Shared_Pointer {
  protected:
      T *pointer = nullptr;
      std::shared_ptr<T> shared_pointer;

  public:
      explicit Optional_Shared_Pointer(T &pointer) : pointer(&pointer) {}

      explicit Optional_Shared_Pointer(std::shared_ptr<T> shared_pointer, T *pointer) :
        shared_pointer(shared_pointer),
        pointer(pointer) {
        if (!pointer)
          throw std::runtime_error("Pointer cannot be null.");
      }

      explicit Optional_Shared_Pointer(std::shared_ptr<T> shared_pointer) :
        shared_pointer(shared_pointer),
        pointer(shared_pointer.get()) {
        if (!pointer)
          throw std::runtime_error("Pointer cannot be null.");
      }

      T &operator*() const {
        return *pointer;
      }

      T *get() const {
        return pointer;

      }

      T *operator->() {
        return pointer;
      }

      const T *operator->() const {
        return pointer;
      }
  };
}