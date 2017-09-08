#include "gtest/gtest.h"
#include <imp_wrapper/Wrapper.h>
#include <utility/utility.h>

using namespace std;

TEST(Main_Test, pizza) {
//  compile("pizza");
//
//  compare(string(RESOURCE_PATH) + "pizza/Pizza.h", string(OUTPUT_PATH) + "Pizza.h");
//  compare(string(RESOURCE_PATH) + "pizza/Pizza.cpp", string(OUTPUT_PATH) + "Pizza.cpp");
}

TEST(Main_Test, game) {
  compile("game");
  Comparison test("game");
  test.compare("Game.cpp");
  test.compare("Game.h");
  test.compare("World.h");
  test.compare("World.cpp");
  test.compare("Creature.h");
  test.compare("Hero.h");
  test.compare("Monster.h");
}

//TEST(Main_Test, generic_dungeon) {
//  compile("generic_dungeon");
//  Comparison test("generic_dungeon");
//  test.compare("House.h");
//  test.compare("General.h");
//  test.compare("General.cpp");
//}
//
//TEST(Main_Test, generic_function) {
//  compile("generic_function");
//  Comparison test("generic_function");
//  test.compare("General.h");
//  test.compare("General.cpp");
//}

//TEST(Main_Test, fail_generic_dungeon) {
//  compile("fail_generic_dungeon");
//}
