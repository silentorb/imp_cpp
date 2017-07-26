#pragma once



class World {
    std::unique_ptr<vector> creatures;

public:
    void add_creature(std::unique_ptr<Creature> &creature);
};