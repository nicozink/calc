/*
Copyright © Nico Zink
All rights reserved.
*/

#pragma once

// Local includes

// Project includes
#include <cpp_util/types/variant_list.h>

// System Includes

class parse_result
{
    public:

    parse_result(VariantList v);

    template <typename ValueType>
    ValueType get();

    private:
    VariantList v;
};

template <typename ValueType>
ValueType parse_result::get()
{
    return v.get<ValueType>(0);
}
