#include <gtest/gtest.h>
#include <cpp_scanning/Header_Scanner.h>
#include <runic_cpp/Cpp_Lexicon.h>

using namespace std;
using namespace cpp_scanning;

TEST(Main_Test, exploring) {
  Header_Scanner scanner;
  runic_cpp::Symbols lexicon;
  runic::Lookup lookup;
  runic::Lookup::initialize<runic::Keywords, runic::Double_Symbols, runic::Single_Symbols>(lookup, lexicon);
  scanner.load_header(string(IMP_ROOT) + "/projects/parse/cpp_scanning/source/cpp_scanning/Header_Scanner.h",
                      {lexicon, lookup});
}
