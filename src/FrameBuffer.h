#pragma once

#include "Context.h"
#include "ComponentType.h"
#include <vector>

namespace rprf
{

class FrameBuffer : public ContextObject<rpr_framebuffer>
{
public:
	FrameBuffer(Context& context, const int width, const int height, int numComponets = 4, ComponentsType type = ComponentsType::Float32);

	size_t data(void* buffer, size_t size) const;
	void data(std::vector<std::byte>* buffer) const;

	void saveToFile(const std::string_view& fileName) const;

	void clear();

	size_t bufferSize() const;

	int numComponents() const            { return m_numComponents; }
	ComponentsType componentType() const { return m_componentType; }

	FrameBuffer clone() const;
private:
	Context& m_context;
	int m_width;
	int m_height;
	int m_numComponents;
	ComponentsType m_componentType;
};

} // namespace
