#pragma once

class World;

class Creature {
    World *world = nullptr;

public:
    void set_world(World &value) {
      world = &value;
    }
};