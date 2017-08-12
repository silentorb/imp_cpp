#include <Creature.h>
#include <World.h>

void World::add_creature(std::unique_ptr<Creature> &creature) {
  creatures.push_back(std::move(creature));
  creature->set_world(this);
}