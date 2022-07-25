#include "Object.h"
#include "Error.h"
#include <RadeonProRender.h>
#include <exception>
#include <iostream>

namespace rprf
{

Object::Object(void* instance) noexcept
: m_instance(instance)
{
}

Object::Object(Object&& object) noexcept
{
	m_instance = object.m_instance;
	object.m_instance = nullptr;
}

Object::~Object()
{
	try {
		destroy();
	}
	catch (const std::exception& e)	{
		std::cerr << "RadeonProRender error: " << e.what() << std::endl;
	}
	catch(...) {
		std::cerr << "RadeonProRender error: unkown exception raised in Object::~Object()" << std::endl;
	}
}


void Object::own(void*&& instance)
{
	m_instance = instance;
	instance = nullptr;
}

void Object::destroy()
{
	if (!m_instance)
		return;

	rpr_int status;
	status = rprObjectDelete(m_instance);
	check(status);

	m_instance = nullptr;
}

Object& Object::operator=(Object&& other)
{
	destroy();

	m_instance = other.m_instance;
	other.m_instance = nullptr;

	return *this;
}

}
