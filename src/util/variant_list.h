#pragma once

#include <vector>

class VariantList
{
    public:

    VariantList();

    ~VariantList();
    
    template <typename T>
	T& get(int i);

    template <typename T>
	void push_back(const T& item);

    int size() const;

    private:

    std::vector<void*> items;
};

template <typename T>
T& VariantList::get(int i)
{
    T* item_ptr = static_cast<T*>(items[i]);
    return *item_ptr;
}

template <typename T>
void VariantList::push_back(const T& item)
{
    T* item_ptr = new T(item);
    items.push_back(item_ptr);
}
