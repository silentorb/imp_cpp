#include <World.h>

void World::add_creature(std::unique_ptr<Hero> &creature) {
  creatures.push_back(std::move(creature));
  creature->set_world(this);
}