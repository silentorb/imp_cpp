#include <Creature.h>
#include <World.h>
#include <Hero.h>
#include <Monster.h>
#include <Game.h>

std::unique_ptr<Game> Game::_new() {
  auto game = std::unique_ptr<Game>(new Game());
  game->world = std::unique_ptr<World>(new World());
  auto hero = std::unique_ptr<Creature>(new Hero());
  game->world->add_creature(std::move(hero));
  game->world->add_creature(std::unique_ptr<Creature>(new Monster()));
  return game;
}