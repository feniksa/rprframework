#include "rprf/Object.h"
#include "rprf/Error.h"
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
	destroy();
}

void Object::destroy() noexcept try
{
	if (!m_instance)
		return;

	rpr_int status;
	status = rprObjectDelete(m_instance);
	check(status);

	m_instance = nullptr;
}
// error
catch (const std::exception& e)	{
	std::cerr << "RadeonProRender error: " << e.what() << "\n";
	m_instance = nullptr;
}
catch(...) {
	std::cerr << "RadeonProRender error: unkown exception raised in Object::~Object()\n";
	m_instance = nullptr;
}

Object& Object::operator=(Object&& other) noexcept
{
	destroy();

	m_instance = other.m_instance;
	other.m_instance = nullptr;

	return *this;
}

}
