#pragma once

#include "RadeonImageFilters.h"
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
	explicit ContextObject(T data = nullptr);
	ContextObject(ContextObject&& object)  noexcept;
	virtual ~ContextObject();

	ContextObject& operator=(ContextObject&& context);
	void setInstance(T&& instance);

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
ContextObject<T>::ContextObject(T dataPointer)
: m_instance(dataPointer)
{
}

template <class T>
ContextObject<T>::~ContextObject()
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
ContextObject<T>& ContextObject<T>::operator=(ContextObject&& context)
{
	if (m_instance)
		destroy();

	m_instance = std::move(context.m_instance);
	context.m_instance = nullptr ;
	return *this;
}

template <class T>
void ContextObject<T>::setInstance(T&& instance)
{
	destroy();
	m_instance = instance;
	instance = nullptr;
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

}
