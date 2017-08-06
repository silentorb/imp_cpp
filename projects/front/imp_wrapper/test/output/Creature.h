#pragma once

#include <memory>
#include <World.h>

class World;

class Creature {
    World *world = nullptr;

public:
    void set_world(std::unique_ptr<World> &value) {
      world = value;
    }
};