#include <gtest/gtest.h>
#include <Header_Scanner.h>

using namespace std;
using namespace cpp_scanning;

TEST(Main_Test, exploring) {
  Header_Scanner scanner;
  scanner.load_header(string(IMP_ROOT) + "/projects/parse/cpp_scanning/source/Header_Scanner.h");
}
