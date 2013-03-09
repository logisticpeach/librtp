#include <atomic>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "../../include/sockets.h"

namespace sockets
{
	std::atomic<bool> g_winsock_initialized = false;
	WSADATA g_winsock_data;

	int InitializeWinsock()
	{
		if (g_winsock_initialized)
			return 0;

		// TODO - implement lock to make this code thread-safe
		auto version = MAKEWORD(2,2);
		int retval = WSAStartup(version, &g_winsock_data);

		if (retval == 0)
		{
			g_winsock_initialized = true;
			return 0;
		}
		else
			return retval;
	}


	int Win32ConnectSocket(std::wstring host, std::wstring service, int sock_handle)
	{
		ADDRINFOW hints;
		ZeroMemory(&hints, sizeof(ADDRINFOW));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		ADDRINFOW* address_results = nullptr;
		ADDRINFOW* current_address = nullptr;

		int retval = GetAddrInfoW(host.c_str(), service.c_str(),
			&hints, &address_results);

		bool connected = false;
		current_address = address_results;

		while (!connected && current_address != nullptr)
		{
			retval = connect(sock_handle, current_address->ai_addr, current_address->ai_addrlen);
			current_address = current_address->ai_next;
			if (retval != SOCKET_ERROR)
			{
				connected = true;
				retval = 0;
			}
		}

		FreeAddrInfoW(address_results);

		return retval;
	}

	SocketBase::SocketBase()
	{
		m_socket_open = false;
		m_socket_connected = false;
		m_socket_handle =  INVALID_SOCKET;
		m_last_error = 0;
		InitializeWinsock();
	}

	SocketBase::~SocketBase()
	{
		CloseSocket();
	}

	SocketError SocketBase::GetError()
	{
		SocketError no_error;
		no_error.error_code = 0;
		no_error.error_type = SocketErrorType::SUCCESS;
		return no_error;
	}

	bool SocketBase::IsConnected()
	{
		return m_socket_connected;
	}

	void SocketBase::CloseSocket()
	{
		if (m_socket_connected)
		{
			shutdown(m_socket_handle, SD_BOTH);
			m_socket_connected = false;
		}
		if (m_socket_open || m_socket_handle != INVALID_SOCKET)
		{
			closesocket(m_socket_handle);
			m_socket_open = false;
			m_socket_handle = INVALID_SOCKET;
		}
	}

	void StreamSocket::Connect(std::wstring host, std::wstring service)
	{
		m_socket_handle = socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket_handle ==  INVALID_SOCKET)
		{
			m_socket_open = false;
			return;
		}

		int retval = Win32ConnectSocket(host, service, m_socket_handle);
		if (retval == 0)
		{
			m_socket_open = true;
			m_socket_connected = true;
		}

	}
}