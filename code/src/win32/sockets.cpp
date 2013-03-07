#include <WinSock2.h>
#include "../../include/sockets.h"

namespace sockets
{
	/*
			bool m_socket_open;
		bool m_socket_connected;
		*/
	SocketBase::SocketBase()
	{
		m_socket_open = false;
		m_socket_handle =  INVALID_SOCKET;
	}

	void SocketBase::ConnectSocket()
	{
		m_socket_handle = socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket_handle ==  INVALID_SOCKET)
			m_socket_open = true;
	}

	bool SocketBase::IsConnected()
	{
		return m_socket_connected;
	}

	void SocketBase::CloseSocket()
	{
		closesocket(m_socket_handle);
		m_socket_open = false;
		m_socket_connected = false;
		m_socket_handle = INVALID_SOCKET;
	}

	StreamSocket::StreamSocket() : SocketBase()
	{
		ConnectSocket();
	}

	StreamSocket::~StreamSocket()
	{
		CloseSocket();
	}
}