#include <gtest/gtest.h>
#include <cpp_scanning/Header_Scanner.h>
#include <runic_cpp/Cpp_Lexicon.h>
#include <imp_summoning/Zookeeper.h>
#include <cpp_stl/Standard_Library.h>

using namespace std;
using namespace cpp_scanning;

TEST(Main_Test, exploring) {
  underworld::Dungeon root_dungeon("", nullptr);
  imp_summoning::Zookeeper zookeeper(root_dungeon);
  cpp_stl::Standard_Library stl;
  stl.initialize_underworld(zookeeper);
  Header_Scanner scanner(root_dungeon);
  runic_cpp::Symbols lexicon;
  runic::Lexer_Lookup lookup;
  runic::Lexer_Lookup::initialize<runic_cpp::Keywords, runic_cpp::Double_Symbols, runic_cpp::Single_Symbols>(lookup, lexicon);
  scanner.load_header(string(IMP_ROOT) + "/projects/parse/cpp_scanning/source/cpp_scanning/Header_Scanner.h",
                      {lexicon, lookup});
}
