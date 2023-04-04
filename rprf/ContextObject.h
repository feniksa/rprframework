#pragma once

#include <type_traits>
#include <stdexcept>

namespace rprf
{
    void __rprObjectDelete(void*) noexcept;
    void __rprObjectSetName(void*, const char*);
    void __rprObjectSetCustomPointer (void*, void*);
    const void* __rprObjectGetCustomPointer (void*);
}

namespace rprf
{

template <class T>
class ContextObject
{
public:
	explicit ContextObject(T data = nullptr) noexcept;
	ContextObject(ContextObject&& object)  noexcept;

	virtual ~ContextObject() noexcept;

	ContextObject& operator=(ContextObject&& context);

	// be carefull! No delete of instance
	[[deprecated]]
	void setInstanceToNull() { m_instance = nullptr; }

	T& instance() noexcept { return m_instance; }
	const T& instance() const noexcept { return m_instance; }

	operator T() const noexcept { return m_instance; }

    void setName(const char* name);
    const void* getCustomPointer() const;

	void destroy() noexcept;

	// we don't allow to copy context object
	ContextObject(const ContextObject&)            = delete;
	ContextObject& operator=(const ContextObject&) = delete;

protected:
    void setCustomPointer(void* pointer);
    void setInstance(T&& instance);

private:
	T m_instance;
};

template <class T>
ContextObject<T>::ContextObject(T dataPointer) noexcept
: m_instance(dataPointer)
{
    static_assert(std::is_pointer<T>::value);
}

template <class T>
ContextObject<T>::~ContextObject() noexcept
{
    destroy();
}

template <class T>
ContextObject<T>::ContextObject(ContextObject&& object) noexcept
{
	m_instance = object.m_instance;
	object.m_instance = nullptr;

    setCustomPointer(this);
}

template <class T>
ContextObject<T>& ContextObject<T>::operator=(ContextObject&& object)
{
	destroy();

    std::swap(m_instance, object.m_instance);

#ifndef NDEBUG
    object.setCustomPointer(nullptr);
#endif
    setCustomPointer(this);

	return *this;
}

template <class T>
void ContextObject<T>::setInstance(T&& instance)
{
	destroy();

    std::swap(m_instance, instance);
}

template <class T>
void ContextObject<T>::destroy() noexcept
{
	if (!m_instance)
		return;

    __rprObjectDelete(m_instance);

	m_instance = nullptr;
}

template <class T>
void ContextObject<T>::setName(const char* name)
{
    __rprObjectSetName(m_instance, name);
}

template <class T>
void ContextObject<T>::setCustomPointer(void *pointer)
{
    __rprObjectSetCustomPointer(m_instance, pointer);
}

template <class T>
const void* ContextObject<T>::getCustomPointer() const
{
    return __rprObjectGetCustomPointer(m_instance);
}

} // namespace
