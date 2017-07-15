#include <World.h>

#include <Creature.h>

#include <Game.h>

Game::Game(std::unique_ptr <World> world) :
  world(std::move(world)) {}

Game *Game::_new() {
  auto game = new Game(std::unique_ptr<World>(new World()));
  auto hero = std::unique_ptr<Creature>(new Creature());
  game->world->add_creature(hero);
  return game
}