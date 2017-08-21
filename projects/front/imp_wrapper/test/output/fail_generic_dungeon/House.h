#pragma once

#include <memory>

class Princess;

class House {
    std::unique_ptr<Princess> inhabitant;

public:
    void set_inhabitant(std::unique_ptr<Princess> &value);
};