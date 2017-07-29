#pragma once

#include <memory>

class World;

class Creature {
    std::unique_ptr<World> world;

public:
};