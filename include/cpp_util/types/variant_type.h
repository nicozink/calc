#pragma once

#include <memory>
#include <vector>

class VariantType
{
public:

	VariantType();

	~VariantType();

	template <typename T>
	T& get();

	template <typename T>
	void set(const T& item);

private:

	std::shared_ptr<void> variant_item;
};

template <typename T>
T& VariantType::get()
{
	return *std::static_pointer_cast<T>(variant_item);
}

template <typename T>
void VariantType::set(const T& item)
{
	T* item_ptr = new T(item);

	variant_item = std::shared_ptr<T>(item_ptr, [](void* p) {
		delete static_cast<T*>(p);
	});
}
