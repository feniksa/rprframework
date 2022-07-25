#pragma once

#include <ostream>
#include <sstream>

namespace tests
{

class StreamRedirect
{
public:
	StreamRedirect(std::ostream& stream)
	: m_output(),
      m_stream(stream),
      m_oldBuffer(m_stream.rdbuf(m_output.rdbuf()))
	{
	}

	~StreamRedirect()
	{
		restore();
	}

	void restore()
	{
		if (m_oldBuffer) {
			m_stream.rdbuf(m_oldBuffer);
            m_oldBuffer = nullptr;
		}
	}

	std::string buffer() const
	{
		return m_output.str();
	}


	StreamRedirect(const StreamRedirect&)            = delete;
	StreamRedirect& operator=(const StreamRedirect&) = delete;
private:
	std::stringstream m_output;
	std::ostream& m_stream;
	std::streambuf*  m_oldBuffer;
};

} // namespace
