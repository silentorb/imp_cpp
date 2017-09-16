#pragma once

#include <memory>

class Creature;

class World {
    std::unique_ptr<vector<std::unique_ptr<Creature>>> creatures;

public:
    void add_creature(std::unique_ptr<Creature> creature);
};