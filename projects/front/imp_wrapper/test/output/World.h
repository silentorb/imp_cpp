#pragma once



class World {
    vector creatures;

public:
    void add_creature(Creature creature) {
      push_back(creature);
      creature.creature = this;
    }

    World() {}
};