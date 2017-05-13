#pragma once

#include <unordered_map>

namespace imp_mirror {

  class Element_Map {
      std::unordered_map<const void *, void *> elements;

  public:
      template<typename Value, typename Key>
      Value *find_or_null(const Key *key) const {
        auto search = elements.find(key);
        return search != elements.end()
               ? (Value*)(*search).second
               : (Value*)nullptr;
      }

      template<typename Key, typename Value>
      void add(const Key *key, Value *value) {
        elements[key] = value;
      };
  };
}