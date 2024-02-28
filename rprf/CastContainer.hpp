#pragma once

#include "rprf/GlobalFunctions.h"
#include <vector>

namespace rprf
{

template <class T>
class CastContainer
{
public:
    using container_type = std::vector<void*>;

    CastContainer() noexcept = default;
    explicit CastContainer(size_t count) noexcept : m_container(count) {}

    explicit CastContainer(const std::vector<void*>& rawPointers)
    : m_container(rawPointers)
    {
    }

    explicit CastContainer(std::vector<void*>&& rawPointers)
    : m_container(std::move(rawPointers))
    {
    }

    CastContainer(CastContainer&& other) noexcept            = default;
    CastContainer& operator=(CastContainer&& other) noexcept = default;

	[[nodiscard]]
    size_t size() const noexcept { return m_container.size(); }

    container_type::value_type* data()
	{
        return m_container.data();
    }

    T* ref(void* rprObject)
    {
        const void* ptr = sdk::rprObjectGetCustomPointer(rprObject);
        void* p = const_cast<void*>(ptr);
        return reinterpret_cast<T*>(p);
    }

    const T* cref(void* rprObject) const
    {
        const void* ptr = sdk::rprObjectGetCustomPointer(rprObject);
        return reinterpret_cast<const T*>(ptr);
    }

    T* operator[](size_t index) noexcept
    {
        return ref(m_container[index]);
    }
    const T* operator[](size_t index) const noexcept { return cref(m_container[index]); }

    struct Iterator;
    struct ConstIterator;

    Iterator begin() noexcept { return Iterator(m_container.begin()); }
    Iterator end() noexcept { return Iterator(m_container.end()); }

    ConstIterator begin() const noexcept { return ConstIterator(m_container.begin()); }
    ConstIterator end() const noexcept { return ConstIterator(m_container.end()); }
private:
    container_type m_container;
};

template <class T>
struct CastContainer<T>::Iterator : public CastContainer<T>::container_type::iterator
{
    using iter_impl = CastContainer<T>::container_type::iterator;

    Iterator()                noexcept = default;
    Iterator(Iterator&&)      noexcept = default;
    Iterator(const Iterator&) noexcept = default;

    explicit Iterator(const iter_impl& iter) : iter_impl(iter) {}
    explicit Iterator(iter_impl&& iter) : iter_impl(iter) {}

    Iterator& operator=(const Iterator&) noexcept = default;
    Iterator& operator=(Iterator&&)      noexcept = default;

    T* operator->() noexcept
    {
        void* rprObject = iter_impl::operator*();
        const void* ptr = sdk::rprObjectGetCustomPointer(rprObject);
        void* p = const_cast<void*>(ptr);
        return reinterpret_cast<T*>(p);
    }

    T& operator*() noexcept
    {
        void* rprObject = iter_impl::operator*();
        const void* ptr = sdk::rprObjectGetCustomPointer(rprObject);
        return *reinterpret_cast<T*>(ptr);
    }
};

template <class T>
struct CastContainer<T>::ConstIterator : public CastContainer<T>::container_type::const_iterator
{
    using iter_impl = CastContainer<T>::container_type::const_iterator;

    ConstIterator()                     noexcept = default;
    ConstIterator(ConstIterator&&)      noexcept = default;
    ConstIterator(const ConstIterator&) noexcept = default;

    explicit ConstIterator(const iter_impl& iter) : iter_impl(iter) {}
    explicit ConstIterator(iter_impl&& iter) : iter_impl(iter) {}

    ConstIterator& operator=(const ConstIterator&) noexcept = default;
    ConstIterator& operator=(ConstIterator&&)      noexcept = default;

    const T* operator->() noexcept
    {
        void* rprObject = iter_impl::operator*();
        const void* ptr = sdk::rprObjectGetCustomPointer(rprObject);
        //void* p = const_cast<void*>(ptr);
        return reinterpret_cast<const T*>(ptr);
    }

    const T& operator*() noexcept
    {
        void* rprObject = iter_impl::operator*();
        const void* ptr = sdk::rprObjectGetCustomPointer(rprObject);
        return *reinterpret_cast<const T*>(ptr);
    }
};

}