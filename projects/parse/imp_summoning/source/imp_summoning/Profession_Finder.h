#pragma once

#include <underworld/schema/Profession.h>
#include <underworld/schema/Scope.h>
#include "Stream.h"

namespace imp_summoning {

//  class Profession_Finder {
////      std::vector<std::string> path_buffer;
////      int progress = 0;
//      const runic_imp::Symbols &lexicon;
////      std::string *current_string = nullptr;
//
//      underworld::Profession *find(underworld::Scope *scope, const runic_imp::Symbols &lexicon, Stream &input);
////      void prepare_path_buffer(Stream &input);
//      Stream &input;
//
//  public:
//      Profession_Finder(const runic_imp::Symbols &lexicon, Stream &input) : lexicon(lexicon), input(input) {}
//
//      underworld::Profession *find(underworld::Scope &scope, Stream &input);
//
////      const std::string get_failure_path() const;
//  };

  underworld::Profession_Owner find_profession(underworld::Scope &current_scope, const runic_imp::Symbols &lexicon,
                                               Stream &input);

}