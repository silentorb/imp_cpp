#pragma once

#include "gtest/gtest.h"
#include <boost/filesystem/operations.hpp>
#include <windows.h>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <imp_mirror/Temporary_Interface_Manager.h>
#include <underworld/schema/Dungeon.h>

using namespace std;

const std::string load_file(const std::string &file_path) {
  ifstream input(file_path);
  if (!input.is_open())
    throw runtime_error("Could not open " + file_path);

  stringstream stream;
  stream << input.rdbuf();
  return stream.str();
}

void _compare(const std::string &first_file, const std::string &second_file) {
  auto first = load_file(first_file);
  auto second = load_file(second_file);
  if (first == second) {
    EXPECT_EQ(first, second);
  }
  else {
    auto arguments = " /x /s " + second_file + " " + first_file;
    auto command = "\"" + string(DIFF_VIEWER_PATH) + "\"" + arguments;
    std::cout << std::endl << command << std::endl;
    STARTUPINFO info = {sizeof(info)};
    PROCESS_INFORMATION processInfo;
    if (CreateProcess(nullptr, const_cast<char *>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &info,
                      &processInfo)) {
    }
    EXPECT_EQ(first, second);
  }
}

class Comparison {
    std::string name;

public:
    Comparison(const string &name) : name(name) {}

    void compare(const std::string &filename) {
      _compare(string(RESOURCE_PATH) + name + "/" + filename, string(OUTPUT_PATH) + name + "/source/" + filename);
    }
};

void compile(const std::string &input_name) {
  string output_folder = string(OUTPUT_PATH) + '/' + input_name;
  auto full_output_path = output_folder + "/source";
  boost::filesystem::create_directory(string(OUTPUT_PATH));
  boost::filesystem::create_directory(output_folder);
  boost::filesystem::create_directory(full_output_path);
  imp_wrapper::Wrapper wrapper;
  imp_mirror::Temporary_Interface_Manager temporary_interface_manager;
  underworld:: Dungeon underworld_root("", nullptr);
  wrapper.load_file(string(RESOURCE_PATH) + input_name + '/' + input_name + ".imp", underworld_root);
  wrapper.mirror(temporary_interface_manager, underworld_root);
  wrapper.solve();
  wrapper.render(full_output_path);
  std::ofstream output_stream(output_folder + "/CMakeLists.txt", std::ios_base::binary | std::ios_base::out);
  if (output_stream.is_open()) {
    output_stream << "create_library(generated_" + input_name + "_test)";
    output_stream.close();
  }
}