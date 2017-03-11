#include "gtest/gtest.h"
#include <imp_wrapper/Wrapper.h>
#include <boost/filesystem/operations.hpp>
#include <handleapi.h>
#include <processthreadsapi.h>

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
//    system(command);
    STARTUPINFO info = {sizeof(info)};
    PROCESS_INFORMATION processInfo;
    if (CreateProcess(nullptr, const_cast<char *>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo)) {
//      WaitForSingleObject(processInfo.hProcess, INFINITE);
//      CloseHandle(processInfo.hProcess);
//      CloseHandle(processInfo.hThread);
    }
    EXPECT_EQ(first, second);
  }
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
