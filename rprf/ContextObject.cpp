#include "rprf/ContextObject.h"
#include "rprf/GlobalFunctions.h"
#include <algorithm>

namespace rprf
{

ContextObject::ContextObject(void* dataPointer) noexcept
		: m_instance(dataPointer)
{
}

ContextObject::~ContextObject() noexcept
{
	destroy();
}

ContextObject::ContextObject(ContextObject&& object) noexcept
{
	m_instance = object.m_instance;
	object.m_instance = nullptr;

	setCustomPointer(this);
}

ContextObject& ContextObject::operator=(ContextObject&& object) noexcept
{
	if (&object == this)
		return *this;

	destroy();

	std::swap(m_instance, object.m_instance);

#ifndef NDEBUG
	object.setCustomPointer(nullptr);
#endif
	setCustomPointer(this);

	return *this;
}

void ContextObject::setInstance(void* instance)
{
	destroy();

	m_instance = instance;
}

void ContextObject::destroy() noexcept
{
	if (!m_instance)
		return;

	sdk::rprObjectDelete(m_instance);

	m_instance = nullptr;
}

void ContextObject::setName(const char* name)
{
	sdk::rprObjectSetName(m_instance, name);
}

void ContextObject::setCustomPointer(void *pointer)
{
	sdk::rprObjectSetCustomPointer(m_instance, pointer);
}

const void* ContextObject::getCustomPointer() const
{
	return sdk::rprObjectGetCustomPointer(m_instance);
}

}
