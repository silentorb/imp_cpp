#include <Creature.h>

#include <World.h>

void World::add_creature(std::unique_ptr<Creature> &creature) {
  push_back(creature);
  creature.creature = this;
}