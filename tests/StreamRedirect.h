#pragma once

#include <ostream>
#include <sstream>

namespace tests
{

class StreamRedirect
{
public:
	StreamRedirect(std::ostream& stream);
	~StreamRedirect();

	void restore();

	std::string str() const;

	StreamRedirect(const StreamRedirect&)            = delete;
	StreamRedirect& operator=(const StreamRedirect&) = delete;
private:
	std::stringstream m_output;
	std::ostream& m_stream;
	std::streambuf*  m_oldBuffer;
};

} // namespace tests
