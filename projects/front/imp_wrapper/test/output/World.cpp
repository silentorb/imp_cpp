#include <Creature.h>

#include <World.h>

unknown World::add_creature(Creature creature) {
  push_back(creature);
  creature.creature = this;
}