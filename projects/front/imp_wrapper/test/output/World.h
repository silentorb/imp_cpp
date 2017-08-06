#pragma once

#include <vector>
#include <memory>

class Hero;

class World {
    std::vector creatures;

public:
    void add_creature(std::unique_ptr<Hero> &creature);
};