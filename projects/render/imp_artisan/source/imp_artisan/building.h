#pragma once

#include "Stroke.h"

namespace imp_artisan {
  namespace building {

    class Stroke_Owner_Stream : public Stroke_Owner {
    public:
        Stroke_Owner_Stream(internal::Stroke *stroke) : Stroke_Owner(stroke) {}

    };

//    class Block : public Stroke_Owner_Stream {
//    public:
//        Block(const std::string &header)
//          : Stroke_Owner_Stream(new internal::Standard_Block(header)) {}
//
//    };
  }
}