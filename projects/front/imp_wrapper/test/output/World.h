#pragma once



class World {
    vector creatures;

public:
    void add_creature(std:: unique_ptr<Creature> &creature);

    World() {}
};