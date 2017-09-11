#include <gtest/gtest.h>
#include <cpp_scanning/Header_Scanner.h>
#include <runic_cpp/Cpp_Lexicon.h>

using namespace std;
using namespace cpp_scanning;

TEST(Main_Test, exploring) {
  underworld::Dungeon root_dungeon("", nullptr);
  Header_Scanner scanner(root_dungeon);
  runic_cpp::Symbols lexicon;
  runic::Lexer_Lookup lookup;
  runic::Lexer_Lookup::initialize<runic_cpp::Keywords, runic_cpp::Double_Symbols, runic_cpp::Single_Symbols>(lookup, lexicon);
  scanner.load_header(string(IMP_ROOT) + "/projects/parse/cpp_scanning/source/cpp_scanning/Header_Scanner.h",
                      {lexicon, lookup});
}
