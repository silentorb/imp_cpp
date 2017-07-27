#include <World.h>

#include <Creature.h>

#include <Game.h>

std::unique_ptr<Game> Game::new() {
  world = World();
  auto game = Game();
  auto hero = Creature();
  game->world->add_creature(hero);
  return game;
}