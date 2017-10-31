#pragma once

#include "gtest/gtest.h"
#include <boost/filesystem/operations.hpp>
#include <windows.h>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <imp_mirror/Temporary_Interface_Manager.h>
#include <underworld/schema/Dungeon.h>
#include <imp_taskmaster/project_loader.h>

const std::string load_file(const std::string &file_path) {
  std::ifstream input(file_path);
  if (!input.is_open())
    throw std::runtime_error("Could not open " + file_path);

  std::stringstream stream;
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
    auto command = "\"" + std::string(DIFF_VIEWER_PATH) + "\"" + arguments;
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
    Comparison(const std::string &name) : name(name) {}

    void compare(const std::string &filename) {
      _compare(std::string(RESOURCE_PATH) + name + "/" + filename,
               std::string(OUTPUT_PATH) + name + "/source/" + filename);
    }
};

void compile(const std::string &input_name) {
  std::string output_folder = std::string(OUTPUT_PATH) + '/' + input_name;
  auto full_output_path = output_folder + "/source";
  boost::filesystem::create_directory(std::string(OUTPUT_PATH));
  boost::filesystem::create_directory(output_folder);
  boost::filesystem::create_directory(full_output_path);
  imp_wrapper::Wrapper wrapper;
  imp_mirror::Temporary_Interface_Manager temporary_interface_manager;
  underworld::Dungeon underworld_root("", nullptr);
  wrapper.load_file(std::string(RESOURCE_PATH) + input_name + '/' + input_name + ".imp", underworld_root);
  wrapper.mirror(temporary_interface_manager, underworld_root);
  wrapper.solve();
  wrapper.render(full_output_path);
  std::ofstream output_stream(output_folder + "/CMakeLists.txt", std::ios_base::binary | std::ios_base::out);
  if (output_stream.is_open()) {
    output_stream << "create_library(generated_" + input_name + "_test)";
    output_stream.close();
  }
}

void build_project(const std::string &path) {
  imp_taskmaster::Project_Map all_projects;
  imp_taskmaster::load_project(path, all_projects);
}