#pragma once

#include <memory>

class Creature;

class World {
    !unknown! creatures;

public:
    void add_creature(std::unique_ptr<Creature> creature);
};