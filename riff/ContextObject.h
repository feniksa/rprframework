#pragma once

#include <RadeonImageFilters.h>
#include "Error.h"
#include <exception>
#include <stdexcept>
#include <iostream>

namespace riff
{

template <class T>
class ContextObject
{
public:
	explicit ContextObject(T data = nullptr) noexcept;
	ContextObject(ContextObject&& object)  noexcept;
	virtual ~ContextObject() noexcept;

	T& instance() noexcept { return m_instance; }
	const T& instance() const noexcept { return m_instance; }

	operator T() const noexcept { return m_instance; }

	virtual void destroy();

	// we don't allow to copy context object
	ContextObject(const ContextObject&)            = delete;
	ContextObject& operator=(const ContextObject&) = delete;

protected:
    ContextObject& operator=(ContextObject&& context);
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
	try {
		destroy();
	} catch(const std::exception& e) {
		std::cerr << "RadeonImageFilter error: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "RadeonImageFilter error: unkown exception raised in ContextObject::~ContextObject()" << std::endl;
	}
}

template <class T>
ContextObject<T>::ContextObject(ContextObject&& object) noexcept
{
	m_instance = object.m_instance;
	object.m_instance = nullptr;
}

template <class T>
ContextObject<T>& ContextObject<T>::operator=(ContextObject&& other)
{
	destroy();
    std::swap(m_instance, other.m_instance);

	return *this;
}

template <class T>
void ContextObject<T>::setInstance(T&& instance)
{
	destroy();

    std::swap(m_instance, instance);
}

template <class T>
void ContextObject<T>::destroy()
{
	if (!m_instance)
		return;

	rif_int status;
	status = rifObjectDelete(m_instance);
	check(status);

	m_instance = nullptr;
}

} // namespace
