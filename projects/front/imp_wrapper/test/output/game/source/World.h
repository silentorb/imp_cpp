#pragma once

#include <memory>
#include <vector>

class Creature;

class World {
    std::vector<!unknown!> creatures;

public:
    void add_creature(std::unique_ptr<Creature> creature);
};