#ifndef LIBRTP_RTPMESSAGES
#define LIBRTP_RTPMESSAGES

#include "buffers.h"

namespace messages
{
	class MessageInterface
	{
	public:
		virtual buffers::Buffer Serialise() = 0;
		virtual unsigned long GetMessageSize() = 0;
	};

	class RtpHeader : public MessageInterface
	{
	public:
		RtpHeader();
		RtpHeader(short version, bool padding, bool has_extensions, short csrc_count); 

		buffers::Buffer Serialise() override;
		unsigned long GetMessageSize() override;

	private:
		unsigned short m_version;
		bool m_padding;
		bool m_extension;
		short m_csrc_count;
	};
}

#endif