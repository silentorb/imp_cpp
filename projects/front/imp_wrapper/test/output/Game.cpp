#include "Game.h"

Game::Game() {
  world = World();
  auto hero = Creature();
  add_creature(hero);
}