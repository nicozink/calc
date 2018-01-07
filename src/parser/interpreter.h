#pragma once

#include "calc.h"

#include <memory>

class Interpreter
{
    struct impl;
    std::unique_ptr<impl> p_impl;

public:

    Interpreter();
    ~Interpreter();

    void add(translation_unit* unit);
    void execute();
};
