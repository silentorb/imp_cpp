#pragma once

class General {


public:
    template <typename A>
    static void kiss(A &kissee) {
      auto &temp = kissee;
    }

    static void main();
};