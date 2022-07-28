#pragma once

namespace rprf
{

class Object
{
public:
	Object(void* instance = nullptr) noexcept;
	Object(Object&& object) noexcept;
	virtual ~Object();

	void destroy();

	Object& operator=(Object&& other);
	void own(void*&& instance);

	Object(const Object&)            = delete;
	Object& operator=(const Object&) = delete;
protected:
	void* instance() const noexcept { return m_instance; }
private:
	void* m_instance;
};

}
