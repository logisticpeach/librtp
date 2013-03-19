#ifndef LIBRTP_BUFFERS
#define LIBRTP_BUFFERS

#include <memory>
#include <vector>

namespace buffers
{
	class Buffer
	{
	public:
		int Read(int offset, int size, char** targetbuffer);
		void Append(const char* data, int size);
		unsigned long Size();
	private:
		std::vector<char> m_buffer;
	};
}











#endif