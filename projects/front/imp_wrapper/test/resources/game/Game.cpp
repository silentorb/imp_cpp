#include <World.h>

#include <Creature.h>

#include <Game.h>

Game::Game() {
  world = std:: unique_ptr<World>(new World());
  auto hero = std:: unique_ptr<Creature>(new Creature());
  world->add_creature(hero);
}