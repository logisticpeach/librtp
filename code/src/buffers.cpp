#include "..\include\buffers.h"
#include <algorithm>

namespace buffers
{
	Buffer::Buffer()
	{
		m_buffer_size = 0;
	}

	Buffer::Buffer(const Buffer& original)
	{
		this->m_buffer_size = 0;
		char * data = original.m_buffer.get();
		Append(data, m_buffer_size);
	}

	unsigned long Buffer::Size()
	{
		return m_buffer_size;
	}

	void Buffer::Append(const char* data, unsigned long size)
	{
		auto new_size = m_buffer_size + size;
		char* x = new char[new_size];
		memset(x, 0, new_size);

		// If we had data, copy it
		
		if (m_buffer.get() != nullptr)
		{
			std::copy(m_buffer.get(), m_buffer.get() + m_buffer_size, x);
		}

		std::copy(data, data + size, x + m_buffer_size);

		m_buffer_size = new_size;
		m_buffer.reset(x);
	}

	int Buffer::Read(unsigned long offset, unsigned long size, char** targetbuffer)
	{
		if (targetbuffer == nullptr)
			return 0;

		if (offset >= m_buffer_size)
			return 0;

		if ((offset + size) > m_buffer_size)
			return 0;

		if (offset < 0 || size <= 0)
			return 0;

		std::copy(m_buffer.get() + offset, m_buffer.get() + (offset + size), *targetbuffer);

		return size;
	}
}