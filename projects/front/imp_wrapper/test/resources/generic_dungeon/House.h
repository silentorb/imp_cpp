#pragma once

#include <memory>

template <typename A>
class House {
    std::unique_ptr<A> inhabitant;

public:
    void set_inhabitant(std::unique_ptr<A> value) {
      inhabitant = value;
    }
};