#include "variant_list.h"

VariantList::VariantList()
{

}

VariantList::~VariantList()
{
    for (auto item_ptr : items)
    {
        delete item_ptr;
    }
}

int VariantList::size() const
{
    return items.size();
}
