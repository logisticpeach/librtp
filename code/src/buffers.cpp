#include "..\include\buffers.h"
#include <algorithm>

namespace buffers
{
	unsigned long Buffer::Size()
	{
		return m_buffer.size();
	}

	void Buffer::Append(const char* data, int size)
	{
		if (data == nullptr)
			return;

		int size_to_reserve = m_buffer.size() + size;

		m_buffer.resize(size_to_reserve);

		std::copy(data, data+size, m_buffer.begin());
	}

	int Buffer::Read(int offset, int size, char** targetbuffer)
	{
		if (targetbuffer == nullptr)
			return 0;

		if (offset >= m_buffer.size())
			return 0;

		if ((offset + size) > m_buffer.size())
			return 0;

		if (offset < 0 || size <= 0)
			return 0;

		std::copy(m_buffer.begin() + offset, m_buffer.begin() + (offset + size), *targetbuffer);

		return size;
	}
}