#include <runic/common/Common_Lexer.h>
#include <runic/File_Text_Source.h>
#include "Header_Scanner.h"
#include "Header_Parser.h"

using namespace runic;

namespace cpp_scanning {

  void Header_Scanner::load_header(const std::string &path) {
    Common_Lexer lexer(std::unique_ptr<runic::Text_Source<Char>>(new runic::File_Text_Source<>(path)));
    auto source_file = new source_mapping::Source_File(path);
    source_files.push_back(std::unique_ptr<source_mapping::Source_File>(source_file));
    Stream stream(lexer, *source_file);
    Header_Parser parser(dungeons, stream);
    parser.parse();
  }
}