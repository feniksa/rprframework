#pragma once

#include "RadeonProRender.h"
#include "Error.h"
#include <exception>
#include <stdexcept>
#include <iostream>

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
	void setInstance(T&& instance);

	// be carefull! No delete of instance
	[[deprecated]]
	void setInstanceToNull() { m_instance = nullptr; }

	T& instance() noexcept { return m_instance; }
	const T& instance() const noexcept { return m_instance; }

	operator T() const noexcept { return m_instance; }

	void destroy();

	// we don't allow to copy context object
	ContextObject(const ContextObject&)            = delete;
	ContextObject& operator=(const ContextObject&) = delete;

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
		std::cerr << "RadeonProRender error: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "RadeonProRender error: unkown exception raised in ContextObject::~ContextObject()" << std::endl;
	}
}

template <class T>
ContextObject<T>::ContextObject(ContextObject&& object) noexcept
{
	m_instance = object.m_instance;
	object.m_instance = nullptr;
}

template <class T>
ContextObject<T>& ContextObject<T>::operator=(ContextObject&& context)
{
	destroy();

    std::swap(m_instance, context.m_instance);

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

	rpr_int status;
	status = rprObjectDelete(m_instance);
	check(status);

	m_instance = nullptr;
}

} // namespace
