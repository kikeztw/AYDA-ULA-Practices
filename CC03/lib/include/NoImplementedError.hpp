#pragma once

#include <stdexcept>

class NoImplementedError: public std::runtime_error
{
public:
    NoImplementedError() : std::runtime_error("Operation not implemented yet.") {}
};
