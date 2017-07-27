#pragma once



class World {
    std::vector creatures;

public:
    void add_creature(std::unique_ptr<Creature> &creature);
};