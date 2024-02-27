#include "StreamRedirect.h"

namespace tests
{

StreamRedirect::StreamRedirect(std::ostream& stream)
	: m_output(),
	m_stream(stream),
	m_oldBuffer(m_stream.rdbuf(m_output.rdbuf()))
{
}

StreamRedirect::~StreamRedirect()
{
	restore();
}

void StreamRedirect::restore()
{
	if (m_oldBuffer) {
		m_stream.rdbuf(m_oldBuffer);
		m_oldBuffer = nullptr;
	}
}

std::string StreamRedirect::str() const
{
	return m_output.str();
}

}