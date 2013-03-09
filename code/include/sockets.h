#ifndef LIBRTP_SOCKETS
#define LIBRTP_SOCKETS

#include <string>


namespace sockets
{
	enum SocketErrorType { SUCCESS, PLATFORM_ERROR, NETWORK_ERROR };

	struct SocketError
	{
		SocketErrorType error_type;
		int error_code;
	};

	class SocketBase
	{
	public:
		SocketBase();
		~SocketBase();
		bool IsConnected();
		virtual void Connect(std::wstring host, std::wstring service) = 0;
		SocketError GetError();

	protected:
		int m_socket_handle;
		bool m_socket_open;
		bool m_socket_connected;
	private:
		void CloseSocket();
		int m_last_error;
	};

	
	class StreamSocket : public SocketBase
	{
	public:
		void Connect(std::wstring host, std::wstring service) override;
	};
}

#endif