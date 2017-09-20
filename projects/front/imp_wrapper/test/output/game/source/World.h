#pragma once

#include <memory>
#include <vector>

class Creature;

class World {
    std::vector<std::unique_ptr<Creature>> creatures;

public:
    void add_creature(std::unique_ptr<Creature> creature);
};