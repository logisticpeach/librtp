#ifndef LIBRTP_BUFFERS
#define LIBRTP_BUFFERS

#include <memory>

namespace buffers
{
	class Buffer
	{
	public:
		Buffer();
		Buffer(const Buffer& original);
		int Read(unsigned long offset, unsigned long size, char** targetbuffer);
		void Append(const char* data, unsigned long size);
		unsigned long Size();
	private:

		std::unique_ptr<char> m_buffer;
		unsigned long m_buffer_size;
	};
}

#endif