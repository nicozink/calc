#pragma once

#include <memory>
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

    std::vector<std::shared_ptr<void> > items;
};

template <typename T>
T& VariantList::get(int i)
{
    std::shared_ptr<T> ptr = std::static_pointer_cast<T>(items[i]);

    return *ptr;
}

template <typename T>
void VariantList::push_back(const T& item)
{
    T* item_ptr = new T(item);

    std::shared_ptr<void> ptr(item_ptr, [](void* p) {
        delete static_cast<T*>(p);
    } );

    items.push_back(ptr);
}
