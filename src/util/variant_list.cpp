#include "variant_list.h"

VariantList::VariantList()
{

}

VariantList::~VariantList()
{
    items.clear();
}

int VariantList::size() const
{
    return items.size();
}
