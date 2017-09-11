#pragma once

#include <runic/common/Common_Lexicon.h>

namespace runic_cpp {

  struct Keywords : public runic::Keywords {
      runic::Whisper Inline = "inline";
      runic::Whisper Namespace = "namespace";
      runic::Whisper Class = "class";
      runic::Whisper Struct = "struct";
      runic::Whisper Const = "const";
      runic::Whisper Public = "public";
      runic::Whisper Private = "private";
      runic::Whisper Protected = "protected";
  };

  struct Single_Symbols : public runic::Single_Symbols {
      runic::Whisper pound_sign = "#";
  };

  struct Double_Symbols : public runic::Double_Symbols {
      runic::Whisper namespace_delimiter = "::";
  };

  struct Symbols : public Keywords,
                   public Double_Symbols,
                   public Single_Symbols,
                   public runic::Special_Symbols,
                   public runic::Dynamic_Whispers {
  };

}