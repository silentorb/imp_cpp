#include <runic/File_Text_Source.h>
#include "Zookeeper.h"
#include "Summoner.h"

namespace imp_summoning {

  Zookeeper::Zookeeper() :
    lookup(lexicon) {
    runic::Lexer_Lookup::initialize<runic::Keywords, runic::Double_Symbols, runic::Single_Symbols>
      (lexer_lookup, lexicon);
  }

  void Zookeeper::load_file(const std::string &path, underworld::Dungeon &underworld_root) {
    runic::Common_Lexer<runic_imp::Symbols> lexer(
      std::unique_ptr<runic::Text_Source<runic::Char>>(new runic::File_Text_Source<>(path)), lexicon, lexer_lookup);
    auto source_file = new source_mapping::Source_File(path);
    source_files.push_back(std::unique_ptr<source_mapping::Source_File>(source_file));
    runic::Stream stream(lexer, *source_file);
    imp_summoning::Summoner summoner(stream, lookup);
    summoner.summon(underworld_root);
  }

}