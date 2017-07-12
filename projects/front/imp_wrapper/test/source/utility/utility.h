#pragma once
#include "gtest/gtest.h"
#include <boost/filesystem/operations.hpp>
#include <windows.h>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <imp_mirror/Temporary_Interface_Manager.h>

using namespace std;

const std::string load_file(const std::string &file_path) {
  ifstream input(file_path);
  if (!input.is_open())
    throw runtime_error("Could not open " + file_path);

  stringstream stream;
  stream << input.rdbuf();
  return stream.str();
}

void compare(const std::string &first_file, const std::string &second_file) {
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
    if (CreateProcess(nullptr, const_cast<char *>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo)) {
    }
    EXPECT_EQ(first, second);
  }
}

void compile(const std::string &input_file){
  boost::filesystem::create_directory(string(OUTPUT_PATH));
  imp_wrapper::Wrapper wrapper;
  imp_mirror::Temporary_Interface_Manager temporary_interface_manager;
  wrapper.load_file(string(RESOURCE_PATH) + input_file);
  wrapper.mirror(temporary_interface_manager);
  wrapper.solve();
  wrapper.render(string(OUTPUT_PATH));
}