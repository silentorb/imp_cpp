#include "gtest/gtest.h"
#include <imp_wrapper/Wrapper.h>
#include <boost/filesystem/operations.hpp>

using namespace std;


const std::string load_file(const std::string &file_path) {
  ifstream input(file_path);
  if (!input.is_open())
    throw runtime_error("Could not open " + file_path);

  stringstream stream;
  stream << input.rdbuf();
  return stream.str();
}

void compare(const std::string &first, const std::string &second) {
  auto first_text = load_file(first);
  auto second_text = load_file(second);
  EXPECT_EQ(first_text, second_text);
}

TEST(Main_Test, full_process) {
  boost::filesystem::create_directory(string(OUTPUT_PATH));

  imp_wrapper::Wrapper wrapper;
  wrapper.load_file(string(RESOURCE_PATH) + "pizza.imp");
  wrapper.mirror();
  wrapper.render(string(OUTPUT_PATH));

  compare(string(RESOURCE_PATH) + "Pizza.h", string(OUTPUT_PATH) + "Pizza.h");
  compare(string(RESOURCE_PATH) + "Pizza.cpp", string(OUTPUT_PATH) + "Pizza.cpp");
}
