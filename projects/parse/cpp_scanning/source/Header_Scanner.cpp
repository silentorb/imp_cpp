#include <runic_cpp/Cpp_Lexer.h>
#include <runic/File_Text_Source.h>
#include "Header_Scanner.h"
#include "Stream.h"

using namespace runic_cpp;

namespace cpp_scanning {

  void Header_Scanner::load_header(const std::string &path) {
    Cpp_Lexer lexer(std::unique_ptr<runic::Text_Source<Char>>(new runic::File_Text_Source<>(path)));
    auto source_file = new source_mapping::Source_File(path);
    source_files.push_back(std::unique_ptr<source_mapping::Source_File>(source_file));
    Stream stream(lexer, *source_file);
  }
}